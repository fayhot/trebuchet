#include <application.hpp>

Application::Application(uint32_t port)
    : m_screen_resolution(1920.0, 1080.0),
      m_screen_size(0.698, 0.392),
      m_liblo_st(std::make_unique<lo::ServerThread>(port)) {}

Application::Application(const Vec2& screen_resolution,
                         const Vec2& screen_size,
                         uint32_t port)
    : m_screen_resolution(screen_resolution),
      m_screen_size(screen_size),
      m_liblo_st(std::make_unique<lo::ServerThread>(port)) {}

void Application::set_screen_resolution(const Vec2& screen_resolution) {
  m_screen_resolution = screen_resolution;
}

void Application::set_screen_size(const Vec2& screen_size) {
  m_screen_size = screen_size;
}

const Vec2& Application::get_screen_resolution() {
  return m_screen_resolution;
}

const Vec2& Application::get_screen_size() {
  return m_screen_size;
}

void Application::start() {
  // simply detect all supported gestures if no recognizers were setup manually
  if (m_recognizers.empty()) {
    m_recognizers.emplace_back(
        std::make_unique<TapRecognizer>(m_screen_resolution, m_screen_size));
    m_recognizers.emplace_back(std::make_unique<LongTapRecognizer>(
        m_screen_resolution, m_screen_size));
    m_recognizers.emplace_back(
        std::make_unique<PinchRecognizer>(m_screen_resolution, m_screen_size));
    m_recognizers.emplace_back(
        std::make_unique<FlingRecognizer>(m_screen_resolution, m_screen_size));
    m_recognizers.emplace_back(
        std::make_unique<SwipeRecognizer>(m_screen_resolution, m_screen_size));
  }

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

std::vector<GestureEventPair> Application::update() {
  {
    std::lock_guard<std::mutex> gestures_guard(m_gestures_mutex);
    for (auto& gesture : m_active_gestures) {
      gesture->update();
    }
  }

  {
    std::lock_guard<std::mutex> tp_guard(m_tp_mutex);
    std::lock_guard<std::mutex> gestures_guard(m_gestures_mutex);
    for (auto& recognizer : m_recognizers) {
      for (auto & [ gesture, event ] : recognizer->update()) {
        if (event == GestureEvent::START || event == GestureEvent::TRIGGER) {
          for (auto& tp : gesture->touch_points()) {
            for (auto& other_recognizer : m_recognizers) {
              if (other_recognizer != recognizer) {
                other_recognizer->invalidate_touch_point(tp);
              }
            }
            m_unhandled_tps.erase(tp);
          }
        }
        add_gesture_event(gesture, event);
      }
    }

    remove_finished_gestures();
  }

  std::lock_guard<std::mutex> gestures_guard(m_gestures_mutex);
  std::vector<GestureEventPair> gesture_events(std::move(m_gesture_events));
  m_gesture_events.clear();
  return gesture_events;
}

void Application::start_bundle(const std::set<uint32_t>& alive_ids) {
  std::lock_guard<std::mutex> tp_guard(m_tp_mutex);
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
}

void Application::set_cursor(int32_t id,
                             const Vec2& pos,
                             const Vec2& velocity,
                             float acceleration) {
  std::lock_guard<std::mutex> tp_guard(m_tp_mutex);
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
}

void Application::end_bundle(int32_t fseq) {
  std::lock_guard<std::mutex> tp_guard(m_tp_mutex);
  std::lock_guard<std::mutex> gestures_guard(m_gestures_mutex);

  for (auto& recognizer : m_recognizers) {
    auto used_tps = recognizer->recognize(m_unhandled_tps);
    for (auto& tp : used_tps) {
      m_unhandled_tps.erase(tp);
    }
  }

  cleanup_inactive_touch_points();
}

void Application::cleanup_inactive_touch_points() {
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

void Application::remove_finished_gestures() {
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

void Application::add_gesture_event(std::shared_ptr<Gesture> gesture,
                                    GestureEvent event) {
  m_gesture_events.emplace_back(std::make_pair(gesture, event));

  if (event == GestureEvent::START) {
    m_active_gestures.emplace(std::move(gesture));
  }
}

Vec2 Application::tuio_to_pixels(const Vec2& pos) const {
  return pos * m_screen_resolution;
}

Vec2 Application::tuio_to_meters(const Vec2& pos) const {
  return pos * m_screen_size;
}
