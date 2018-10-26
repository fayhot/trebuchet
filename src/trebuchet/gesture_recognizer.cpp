#include <gesture_recognizer.hpp>

GestureRecognizer::GestureRecognizer(uint32_t port)
    : m_screen_resolution(1920.0, 1080.0),
      m_screen_size(0.597, 0.336),
      m_liblo_st(std::make_unique<lo::ServerThread>(port)) {}

void GestureRecognizer::set_screen_resolution(const Vec2& screen_resolution) {
  m_screen_resolution = screen_resolution;
}

void GestureRecognizer::set_screen_size(const Vec2& screen_size) {
  m_screen_size = screen_size;
}

const Vec2& GestureRecognizer::get_screen_resolution() {
  return m_screen_resolution;
}

const Vec2& GestureRecognizer::get_screen_size() {
  return m_screen_size;
}

void GestureRecognizer::start() {
  m_liblo_st->add_method(
      "/tuio/2Dcur", nullptr, [this](lo_arg** argv, int argc) {
        // extract the message type
        std::string msg_type(&argv[0]->s);

        if (msg_type == "alive") {
          // fetch the IDs of the cursors that are active
          std::set<uint32_t> alive_ids;
          for (uint32_t i = 0; i < argc - 1; ++i) {
            alive_ids.insert(argv[1 + i]->i);
          }
          this->start_bundle(alive_ids);

        } else if (msg_type == "set") {
          uint32_t id = argv[1]->i;
          this->set_cursor(argv[1]->i, Vec2(argv[2]->f, argv[3]->f),
                           Vec2(argv[4]->f, argv[5]->f), argv[6]->f);

        } else if (msg_type == "fseq") {
          this->end_bundle(argv[1]->i);
        }
      });
  m_liblo_st->start();
}

std::vector<GestureEventPair> GestureRecognizer::update() {
  m_tp_mutex.lock();
  fire_verified_taps();
  fire_verified_flings();
  remove_finished_gestures();
  m_tp_mutex.unlock();

  m_gestures_mutex.lock();
  std::vector<GestureEventPair> gesture_events(std::move(m_gesture_events));
  m_gesture_events.clear();
  m_gestures_mutex.unlock();
  return gesture_events;
}

void GestureRecognizer::start_bundle(const std::set<uint32_t>& alive_ids) {
  m_tp_mutex.lock();
  // remove the touch points that are not alive anymore
  for (auto it = m_touch_points.begin(); it != m_touch_points.end();) {
    if (alive_ids.find(it->first) == alive_ids.end()) {
      // touch point is not alive anymore, mark it as not alive and delete it
      // from the active touch points
      auto tp = it->second;
      tp->end();
      it = m_touch_points.erase(it);
    } else {
      // go on to the next touch point
      ++it;
    }
  }
  m_tp_mutex.unlock();
}

void GestureRecognizer::set_cursor(int32_t id,
                                   const Vec2& pos,
                                   const Vec2& velocity,
                                   float acceleration) {
  m_tp_mutex.lock();
  auto it = m_touch_points.find(id);
  if (it == m_touch_points.end()) {
    // add new touch point
    auto tp = std::make_shared<TouchPoint>(id, pos, velocity, acceleration);
    m_touch_points.insert(std::make_pair(id, tp));
    m_unhandled_tps.insert(tp);
  } else {
    // update already existing touch point
    auto tp = m_touch_points.at(id);
    tp->update(pos, velocity, acceleration);
  }
  m_tp_mutex.unlock();
}

void GestureRecognizer::end_bundle(int32_t fseq) {
  m_tp_mutex.lock();
  m_gestures_mutex.lock();
  detect_taps();
  detect_long_taps();
  detect_double_taps();
  detect_4finger_pinches();
  detect_2finger_pinches();
  detect_flings();
  detect_swipes();

  cleanup_inactive_touch_points();

  m_gestures_mutex.unlock();
  m_tp_mutex.unlock();
}

void GestureRecognizer::detect_flings() {
  // detect flings with one touch point
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;

    // check if the conditions of a fling are met
    if (tuio_to_meters(tp->velocity()).length() >= FLING_MIN_VELOCITY) {
      bool part_of_other_fling = false;
      // check if this fling is part of another fling
      for (auto& fling : m_flings) {
        if (angle(fling->velocity(), tp->velocity()) < FLING_MAX_ANGLE_DIFF &&
            distance(tuio_to_meters(fling->pos()), tuio_to_meters(tp->pos())) <
                FLING_MAX_DISTANCE) {
          if (fling->add_touch_point(tp)) {
            part_of_other_fling = true;
            break;
          }
        }
      }

      // create a new fling gesture if this touch point was not part of another
      // fling
      if (!part_of_other_fling) {
        m_flings.emplace(std::make_shared<Fling>(tp));
      }

      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::detect_taps() {
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    auto dist =
        distance(tuio_to_meters(tp->pos()), tuio_to_meters(tp->start_pos()));
    if (tp->finished() && tp->duration() < TAP_MAX_DURATION &&
        dist < TAP_MAX_DISTANCE) {
      m_possible_taps.emplace(std::make_shared<Tap>(tp));
      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::detect_long_taps() {
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    auto dist =
        distance(tuio_to_meters(tp->pos()), tuio_to_meters(tp->start_pos()));
    if (tp->duration() > LONG_TAP_MIN_DURATION && dist < TAP_MAX_DISTANCE) {
      auto long_tap = std::make_shared<LongTap>(tp);
      add_gesture_event(long_tap, GestureEvent::START);
      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::detect_double_taps() {
  // at least two possible taps are required for a double tap
  if (m_possible_taps.size() < 2) {
    return;
  }

  auto possible_taps = m_possible_taps;
  for (auto&& taps : iter::combinations(possible_taps, 2)) {
    // check if all of these taps are still in the possible taps set
    if (!std::all_of(taps.begin(), taps.end(), [&](auto& tap) {
          return m_possible_taps.find(tap) != m_possible_taps.end();
        })) {
      continue;
    }

    // compute the properties of the tap combination
    auto pause = std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(
                              taps[0]->touch_point()->end_time() -
                              taps[1]->touch_point()->end_time())
                              .count() /
                          1000.0);
    auto dist =
        distance(taps[0]->touch_point()->pos(), taps[1]->touch_point()->pos());

    // check if the two taps form a double tap
    if (pause < DOUBLE_TAP_MAX_PAUSE && dist < DOUBLE_TAP_MAX_DISTANCE) {
      add_gesture_event(std::make_shared<DoubleTap>(taps[0]->touch_point(),
                                                    taps[1]->touch_point()),
                        GestureEvent::TRIGGER);
      m_possible_taps.erase(taps[0]);
      m_possible_taps.erase(taps[1]);
    }
  }
}

void GestureRecognizer::detect_2finger_pinches() {
  // at least two touch points are required for a tw-finger-pinch
  if (m_unhandled_tps.size() < 2) {
    return;
  }

  auto unhandled_tps = m_unhandled_tps;
  for (auto&& touch_points : iter::combinations(unhandled_tps, 2)) {
    // check if all touch points are still unhandled and have not been detected
    // as part of a pinch in a previous iteration
    if (!only_unhandled_tps({touch_points[0], touch_points[1]})) {
      continue;
    }

    if (angle(touch_points[0]->direction(), touch_points[1]->direction()) >=
        PINCH_MIN_ANGLE_BETWEEN_CLUSTERS) {
      auto pinch = std::make_shared<Pinch>(
          std::set<std::shared_ptr<TouchPoint>>{touch_points[0]},
          std::set<std::shared_ptr<TouchPoint>>{touch_points[1]});
      add_gesture_event(pinch, GestureEvent::START);
      m_unhandled_tps.erase(touch_points[0]);
      m_unhandled_tps.erase(touch_points[1]);
    }
  }
}

void GestureRecognizer::detect_4finger_pinches() {
  // at least two four points are required for a four-finger-pinch
  if (m_unhandled_tps.size() < 4) {
    return;
  }

  auto unhandled_tps = m_unhandled_tps;
  for (auto&& touch_points : iter::combinations(unhandled_tps, 4)) {
    // check if all touch points are still unhandled and have not been detected
    // as part of a pinch in a previous iteration
    if (!only_unhandled_tps({touch_points[0], touch_points[1], touch_points[2],
                             touch_points[3]})) {
      continue;
    }

    for (auto indices : PINCH2F_TP_INDICES) {
      auto tp0 = touch_points[indices[0]];
      auto tp1 = touch_points[indices[1]];
      auto tp2 = touch_points[indices[2]];
      auto tp3 = touch_points[indices[3]];

      // check if the angle between the two positions in a cluster have an angle
      // between them that is too large
      if (angle(tp0->pos(), tp1->pos()) > PINCH_MAX_ANGLE_DIFF_IN_CLUSTERS ||
          angle(tp2->pos(), tp3->pos()) > PINCH_MAX_ANGLE_DIFF_IN_CLUSTERS) {
        continue;
      }

      // compute the average direction of each cluster
      auto cluster0_direction = 0.5 * (tp0->direction() + tp1->direction());
      auto cluster1_direction = 0.5 * (tp2->direction() + tp3->direction());

      if (angle(cluster0_direction, cluster1_direction) >=
          PINCH_MIN_ANGLE_BETWEEN_CLUSTERS) {
        // pinch detected, create the gesture event
        auto pinch = std::make_shared<Pinch>(
            std::set<std::shared_ptr<TouchPoint>>{tp0, tp1},
            std::set<std::shared_ptr<TouchPoint>>{tp2, tp3});
        add_gesture_event(pinch, GestureEvent::START);

        // remove the used touch points from the unhandled set
        m_unhandled_tps.erase(tp0);
        m_unhandled_tps.erase(tp1);
        m_unhandled_tps.erase(tp2);
        m_unhandled_tps.erase(tp3);

        // the current four touch points were detected as a 4-finger-pinch and
        // we don't have to check further combinations of these
        break;
      }
    }
  }
}

void GestureRecognizer::detect_swipes() {
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    auto travel = (tp->travel() * m_screen_size).length();  // in m
    if (travel > SWIPE_MIN_TRAVEL && tp->duration() > SWIPE_MIN_DURATION) {
      add_gesture_event(std::make_shared<Swipe>(tp), GestureEvent::START);
      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::cleanup_inactive_touch_points() {
  // remove unhandled touch points that are not active anymore
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    if (tp->finished()) {
      m_touch_points.erase(tp->id());
      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::fire_verified_taps() {
  for (auto it = m_possible_taps.begin(); it != m_possible_taps.end();) {
    auto tap = *it;
    if (tap->time_finished() > DOUBLE_TAP_MAX_PAUSE) {
      add_gesture_event(tap, GestureEvent::TRIGGER);
      it = m_possible_taps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::fire_verified_flings() {
  for (auto it = m_flings.begin(); it != m_flings.end();) {
    auto fling = *it;

    if (fling->age() > FLING_MULTI_FINGER_MAX_TIME_BETWEEN) {
      add_gesture_event(fling, GestureEvent::START);
      it = m_flings.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::remove_finished_gestures() {
  for (auto it = m_active_gestures.begin(); it != m_active_gestures.end();) {
    auto gesture = *it;
    if (gesture->finished()) {
      add_gesture_event(gesture, GestureEvent::END);
      it = m_active_gestures.erase(it);
    } else {
      ++it;
    }
  }
}

bool GestureRecognizer::only_unhandled_tps(
    const std::set<std::shared_ptr<TouchPoint>>& tps) const {
  return std::all_of(tps.begin(), tps.end(), [&](auto& tp) {
    return m_unhandled_tps.find(tp) != m_unhandled_tps.end();
  });
}

void GestureRecognizer::add_gesture_event(std::shared_ptr<Gesture> gesture,
                                          GestureEvent event) {
  m_gesture_events.emplace_back(std::make_pair(gesture, event));

  if (event == GestureEvent::START) {
    m_active_gestures.emplace(std::move(gesture));
  }
}

Vec2 GestureRecognizer::tuio_to_pixels(const Vec2& pos) const {
  return pos * m_screen_resolution;
}

Vec2 GestureRecognizer::tuio_to_meters(const Vec2& pos) const {
  return pos * m_screen_size;
}
