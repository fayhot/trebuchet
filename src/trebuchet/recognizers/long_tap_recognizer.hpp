#pragma once

#include <recognizers/recognizer.hpp>
#include <gestures/gesture_event.hpp>
#include <gestures/long_tap.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;
using GesturePtr = std::shared_ptr<Gesture>;
using LongTapPtr = std::shared_ptr<LongTap>;
using GestureEventPair = std::pair<GesturePtr, GestureEvent>;

class LongTapRecognizer : public Recognizer {
 public:
  LongTapRecognizer(const Vec2& screen_resolution, const Vec2& screen_size);
  std::set<TouchPointPtr> recognize(const std::set<TouchPointPtr>& touch_points) override;
  std::set<GestureEventPair> update() override;
  bool invalidate_touch_point(const TouchPointPtr& touch_point) override;

 protected:
  std::set<LongTapPtr> check_for_long_taps(
      const std::set<TouchPointPtr>& touch_points);
  std::set<GestureEventPair> verified_double_taps();
  std::set<GestureEventPair> verified_taps();

 protected:
  const std::chrono::milliseconds MIN_DURATION{750};
  const double MAX_DISTANCE{0.01};  // in m

 protected:
  std::set<LongTapPtr> m_long_taps;
};
