#include <recognizers/long_tap_recognizer.hpp>

LongTapRecognizer::LongTapRecognizer(const Vec2& screen_resolution,
                                     const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

std::set<TouchPointPtr> LongTapRecognizer::recognize(
    const std::set<TouchPointPtr>& touch_points) {
  std::set<TouchPointPtr> used_tps;

  for (auto& tp : touch_points) {
    auto dist =
        distance(tuio_to_meters(tp->pos()), tuio_to_meters(tp->start_pos()));
    if (tp->duration() > MIN_DURATION && dist < MAX_DISTANCE) {
      used_tps.insert(tp);
      m_long_taps.emplace(std::make_shared<LongTap>(tp));
    }
  }

  return used_tps;
}

std::set<GestureEventPair> LongTapRecognizer::update() {
  std::set<GestureEventPair> events;

  for (auto& long_tap : m_long_taps) {
    events.emplace(std::make_pair(long_tap, GestureEvent::START));
  }
  m_long_taps.clear();

  return events;
}

bool LongTapRecognizer::invalidate_touch_point(
    const TouchPointPtr& touch_point) {
  bool was_tp_used = false;

  for (auto it = m_long_taps.begin(); it != m_long_taps.end();) {
    auto long_tap = *it;
    if (touch_point == long_tap->touch_point()) {
      was_tp_used = true;
      it = m_long_taps.erase(it);
    } else {
      ++it;
    }
  }

  return was_tp_used;
}
