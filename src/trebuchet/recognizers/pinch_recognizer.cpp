#include <recognizers/pinch_recognizer.hpp>

PinchRecognizer::PinchRecognizer(const Vec2& screen_resolution,
                                 const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

std::set<GestureEventPair> PinchRecognizer::update(
    const std::set<TouchPointPtr>& touch_points) {
  return detect_pinches(touch_points);
}

std::set<GestureEventPair> PinchRecognizer::detect_pinches(
    const std::set<TouchPointPtr>& touch_points) {
  std::set<GestureEventPair> events;

  for (auto&& tps : iter::combinations(touch_points, 2)) {
    if (angle(tps[0]->velocity(), tps[1]->velocity()) >= MIN_OPPOSING_ANGLE) {
      std::set<std::pair<TouchPointPtr, TouchPointPtr>> tp_pairs = {
          std::make_pair(tps[0], tps[1])};
      auto pinch = std::make_shared<Pinch>(tp_pairs);
      events.emplace(std::make_pair(pinch, GestureEvent::START));
    }
  }

  return events;
}
