#pragma once

#include <cppitertools/combinations.hpp>

#include <recognizers/recognizer.hpp>
#include <gestures/gesture_event.hpp>
#include <gestures/tap.hpp>
#include <gestures/double_tap.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;
using GesturePtr = std::shared_ptr<Gesture>;
using TapPtr = std::shared_ptr<Tap>;
using GestureEventPair = std::pair<GesturePtr, GestureEvent>;

class TapRecognizer : public Recognizer {
 public:
  TapRecognizer(const Vec2& screen_resolution, const Vec2& screen_size);
  std::set<TouchPointPtr> recognize(
      const std::set<TouchPointPtr>& touch_points) override;
  std::set<GestureEventPair> update() override;
  bool invalidate_touch_point(const TouchPointPtr& touch_point) override;

 protected:
  std::set<GestureEventPair> verified_double_taps();
  std::set<GestureEventPair> verified_taps();

 protected:
  const std::chrono::milliseconds TAP_MIN_DURATION{40};
  const std::chrono::milliseconds TAP_MAX_DURATION{300};
  const double TAP_MAX_DISTANCE{0.01};  // in m

  const double DOUBLE_TAP_MAX_DISTANCE{0.02};  // in m
  const std::chrono::milliseconds DOUBLE_TAP_MAX_PAUSE{200};

 protected:
  std::set<TapPtr> m_taps;
  std::set<TouchPointPtr> m_used_touch_points;
};
