#include <recognizers/swipe_recognizer.hpp>

SwipeRecognizer::SwipeRecognizer(const Vec2& screen_resolution,
                                 const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

bool SwipeRecognizer::recognize(const std::set<TouchPointPtr>& touch_points) {
  std::set<SwipePtr> swipes;
  for (auto& tp : touch_points) {
    if (m_possible_swipes_tps.find(tp) == m_possible_swipes_tps.end()) {
      auto travel = (tp->travel() * m_screen_size).length();  // in m
      auto max_velocity =
          tuio_to_meters(tp->max_velocity()).length();  // in m/s
      if (travel >= MIN_TRAVEL && max_velocity <= MAX_VELOCITY) {
        swipes.emplace(std::make_shared<Swipe>(tp));
        m_possible_swipes_tps.insert(tp);
      }
    }
  }

  m_possible_swipes.insert(swipes.begin(), swipes.end());

  return !swipes.empty();
}

std::set<GestureEventPair> SwipeRecognizer::update() {
  std::set<GestureEventPair> events;

  for (auto it = m_possible_swipes.begin(); it != m_possible_swipes.end();) {
    auto swipe = *it;
    if (swipe->duration() >= MIN_DURATION) {
      events.emplace(std::make_pair(swipe, GestureEvent::START));
      it = m_possible_swipes.erase(it);
      m_possible_swipes_tps.erase(swipe->touch_point());
    } else {
      ++it;
    }
  }

  return events;
}

bool SwipeRecognizer::invalidate_touch_point(const TouchPointPtr& touch_point) {
  bool was_tp_used = false;

  for (auto it = m_possible_swipes.begin(); it != m_possible_swipes.end();) {
    auto swipe = *it;
    auto tps = swipe->touch_points();
    if (tps.find(touch_point) != tps.end()) {
      was_tp_used = true;
      it = m_possible_swipes.erase(it);
    } else {
      ++it;
    }
  }

  return was_tp_used;
}
