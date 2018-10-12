#include <gesture_recognizer.hpp>

GestureRecognizer::GestureRecognizer(uint32_t port)
    : m_screen_resolution(1920.0, 1080.0),
      m_screen_size(0.597, 0.336),
      m_liblo_st(std::make_unique<lo::ServerThread>(port)) {}

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

void GestureRecognizer::update() {
  detect_taps();
  detect_double_taps();
  fire_verified_taps();

  // remove unhandled touch points that are not active for some time
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    if (tp->finished_since() > UNHANDLED_TP_REMOVE_TIME) {
      m_touch_points.erase(tp->id());
      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::start_bundle(const std::set<uint32_t>& alive_ids) {
  // remove the touch points that are not alive anymore
  for (auto it = m_touch_points.begin(); it != m_touch_points.end();) {
    if (alive_ids.find(it->first) == alive_ids.end()) {
      // touch point is not alive anymore, mark it as not alive and delete it
      // from the active touch points
      auto tp = it->second;
      tp->end();
      std::cout << "remove cursor " << it->first << std::endl;
      it = m_touch_points.erase(it);
    } else {
      // go on to the next touch point
      ++it;
    }
  }
}

void GestureRecognizer::set_cursor(int32_t id,
                                   const Vec2& pos,
                                   const Vec2& velocity,
                                   float acceleration) {
  auto it = m_touch_points.find(id);
  if (it == m_touch_points.end()) {
    // add new touch point
    auto tp = std::make_shared<TouchPoint>(id, pos, velocity, acceleration);
    m_touch_points.insert(std::make_pair(id, tp));
    m_unhandled_tps.push_back(tp);
    std::cout << "add cursor " << id << std::endl;
  } else {
    // update already existing touch point
    auto tp = m_touch_points.at(id);
    tp->update(pos, velocity, acceleration);
  }
}

void GestureRecognizer::end_bundle(int32_t fseq) {}

void GestureRecognizer::detect_taps() {
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    auto dist =
        distance(tuio_to_meters(tp->pos()), tuio_to_meters(tp->start_pos()));
    if (tp->finished() && tp->duration() < TAP_MAX_DURATION &&
        dist < TAP_MAX_DISTANCE) {
      m_possible_taps.emplace_back(std::make_shared<Tap>(tp));
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

  for (auto first_it = m_possible_taps.begin();
       first_it != m_possible_taps.end() - 1;) {
    for (auto second_it = first_it + 1; second_it != m_possible_taps.end();) {
      auto first_tap = *first_it;
      auto second_tap = *second_it;

      // compute the properties of the tap combination
      auto pause =
          std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(
                       first_tap->touch_point()->end_time() -
                       second_tap->touch_point()->end_time())
                       .count() /
                   1000.0);
      auto dist = distance(first_tap->touch_point()->pos(),
                           second_tap->touch_point()->pos());

      // check if the two taps form a double tap
      if (pause < DOUBLE_TAP_MAX_PAUSE && dist < DOUBLE_TAP_MAX_DISTANCE) {
        auto double_tap = std::make_shared<DoubleTap>(
            first_tap->touch_point(), second_tap->touch_point());
        std::cout << "DOUBLE TAP" << std::endl;
        first_it = m_possible_taps.erase(first_it);

        // move the iterators to the next valid one
        if (first_it == second_it) {
          first_it = second_it = m_possible_taps.erase(second_it);
        } else {
          second_it = m_possible_taps.erase(second_it);
        }

        // we're finished when the first iterator points to the end
        if (first_it == m_possible_taps.end()) {
          return;
        }

        // go back one element because the iterator is incremented by one after
        // the continue command again
        --first_it;
        continue;
      } else {
        // does not fulfill double tap properties, move on to the next
        // combination
        ++second_it;
      }
    }
    ++first_it;
  }
}

void GestureRecognizer::fire_verified_taps() {
  for (auto it = m_possible_taps.begin(); it != m_possible_taps.end();) {
    auto tap = *it;
    if (tap->time_finished() > DOUBLE_TAP_MAX_PAUSE) {
      std::cout << "TAP" << std::endl;
      it = m_possible_taps.erase(it);
    } else {
      ++it;
    }
  }
}

Vec2 GestureRecognizer::tuio_to_pixels(const Vec2& pos) const {
  return pos * m_screen_resolution;
}

Vec2 GestureRecognizer::tuio_to_meters(const Vec2& pos) const {
  return pos * m_screen_size;
}
