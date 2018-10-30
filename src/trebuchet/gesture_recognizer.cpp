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
  for (auto& gesture : m_active_gestures) {
    gesture->update();
  }
  fire_verified_taps();
  fire_verified_flings();
  fire_verified_pinches();
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
  detect_pinches();
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
    auto pause =
        taps[0]->touch_point()->end_time() - taps[1]->touch_point()->end_time();
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

void GestureRecognizer::detect_pinches() {
  // at least two touch points are required for a pinch
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

    // check if the touch pointer pair is a 2-finger-pinch
    if (angle(touch_points[0]->direction(), touch_points[1]->direction()) >=
        PINCH_MIN_ANGLE_BETWEEN_CLUSTERS) {
      // create the pinch
      auto pinch = std::make_shared<Pinch>(
          std::set<std::shared_ptr<TouchPoint>>{touch_points[0]},
          std::set<std::shared_ptr<TouchPoint>>{touch_points[1]});
      m_unhandled_tps.erase(touch_points[0]);
      m_unhandled_tps.erase(touch_points[1]);

      // check if the detected pinch is part of another already detected pinch
      bool part_of_other_pinch = false;
      for (auto& other : m_pinches) {
        auto ang = angle(pinch->direction(), other->direction());
        auto first_dist =
            std::min(distance(pinch->first_center(), other->first_center()),
                     distance(pinch->first_center(), other->second_center()));
        auto second_dist =
            std::min(distance(pinch->second_center(), other->first_center()),
                     distance(pinch->second_center(), other->second_center()));
        if ((ang <= PINCH_MULTI_FINGER_MAX_ANGLE_DIFF ||
             ang >= M_PI - PINCH_MULTI_FINGER_MAX_ANGLE_DIFF) &&
            first_dist <= PINCH_MULTI_FINGER_MAX_CLUSTER_DISTANCE &&
            second_dist <= PINCH_MULTI_FINGER_MAX_CLUSTER_DISTANCE) {
          other->merge(*pinch);
          part_of_other_pinch = true;
        }
      }

      // store the new pinch gesture which is not part of another pinch
      if (!part_of_other_pinch) {
        m_pinches.emplace(std::move(pinch));
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
  auto now = Clock::now();
  for (auto it = m_possible_taps.begin(); it != m_possible_taps.end();) {
    auto tap = *it;
    auto time_since_finished = now - tap->end_time();
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
    //                  time_since_finished)
    //                      .count() /
    //                  1000.0
    //           << " " << tap->finished() << std::endl;
    if (time_since_finished > DOUBLE_TAP_MAX_PAUSE) {
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

    if (fling->duration() > FLING_MULTI_FINGER_MAX_TIME_BETWEEN) {
      add_gesture_event(fling, GestureEvent::START);
      it = m_flings.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::fire_verified_pinches() {
  for (auto it = m_pinches.begin(); it != m_pinches.end();) {
    auto pinch = *it;

    if (pinch->duration() > PINCH_MULTI_FINGER_MAX_TIME_BETWEEN) {
      add_gesture_event(pinch, GestureEvent::START);
      it = m_pinches.erase(it);
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
