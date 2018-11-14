#include <recognizers/long_tap_recognizer.hpp>

LongTapRecognizer::LongTapRecognizer(const Vec2& screen_resolution,
                                     const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

std::set<GestureEventPair> LongTapRecognizer::update(
    const std::set<TouchPointPtr>& touch_points) {
  std::set<GestureEventPair> events;
  for (auto& tp : touch_points) {
    auto dist =
        distance(tuio_to_meters(tp->pos()), tuio_to_meters(tp->start_pos()));
    if (tp->duration() > MIN_DURATION && dist < MAX_DISTANCE) {
      auto long_tap = std::make_shared<LongTap>(tp);
      events.emplace(std::make_pair(long_tap, GestureEvent::START));
    }
  }
  return events;
}
