#include <recognizers/pinch_recognizer.hpp>

PinchRecognizer::PinchRecognizer(const Vec2& screen_resolution,
                                 const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

bool PinchRecognizer::recognize(const std::set<TouchPointPtr>& touch_points) {
  std::set<PinchPtr> pinches;
  for (auto&& tps : iter::combinations(touch_points, 2)) {
    if (angle(tps[0]->velocity(), tps[1]->velocity()) >= MIN_OPPOSING_ANGLE) {
      std::set<std::pair<TouchPointPtr, TouchPointPtr>> tp_pairs = {
          std::make_pair(tps[0], tps[1])};
      pinches.emplace(std::make_shared<Pinch>(tp_pairs));
    }
  }

  m_pinches.insert(pinches.begin(), pinches.end());

  return !pinches.empty();
}

std::set<GestureEventPair> PinchRecognizer::update() {
  std::set<GestureEventPair> events;

  for (auto& pinch : m_pinches) {
    events.emplace(std::make_pair(pinch, GestureEvent::START));
  }
  m_pinches.clear();

  return events;
}
