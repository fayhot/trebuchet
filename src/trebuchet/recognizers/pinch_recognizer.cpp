#include <recognizers/pinch_recognizer.hpp>

PinchRecognizer::PinchRecognizer(const Vec2& screen_resolution,
                                 const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

std::set<TouchPointPtr> PinchRecognizer::recognize(
    const std::set<TouchPointPtr>& touch_points) {
  std::set<TouchPointPtr> used_tps;
  for (auto&& tps : iter::combinations(touch_points, 2)) {
    if (angle(tps[0]->velocity(), tps[1]->velocity()) >= MIN_OPPOSING_ANGLE) {
      used_tps.insert(tps[0]);
      used_tps.insert(tps[1]);
      std::set<std::pair<TouchPointPtr, TouchPointPtr>> tp_pairs;
      if (tps[0]->start_pos() < tps[1]->start_pos()) {
        tp_pairs.emplace(std::make_pair(tps[0], tps[1]));
      } else {
        tp_pairs.emplace(std::make_pair(tps[1], tps[0]));
      }
      m_pinches.emplace(std::make_shared<Pinch>(tp_pairs));
    }
  }

  return used_tps;
}

std::set<GestureEventPair> PinchRecognizer::update() {
  std::set<GestureEventPair> events;

  for (auto& pinch : m_pinches) {
    events.emplace(std::make_pair(pinch, GestureEvent::START));
  }
  m_pinches.clear();

  return events;
}

bool PinchRecognizer::invalidate_touch_point(const TouchPointPtr& touch_point) {
  bool was_tp_used = false;

  for (auto it = m_pinches.begin(); it != m_pinches.end();) {
    auto pinch = *it;
    auto tps = pinch->touch_points();
    if (tps.find(touch_point) != tps.end()) {
      was_tp_used = true;
      it = m_pinches.erase(it);
    } else {
      ++it;
    }
  }

  return was_tp_used;
}
