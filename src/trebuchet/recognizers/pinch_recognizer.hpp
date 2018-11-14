#pragma once

#include <cppitertools/combinations.hpp>

#include <recognizers/recognizer.hpp>
#include <gestures/gesture_event.hpp>
#include <gestures/pinch.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;
using GesturePtr = std::shared_ptr<Gesture>;
using PinchPtr = std::shared_ptr<Pinch>;
using GestureEventPair = std::pair<GesturePtr, GestureEvent>;

class PinchRecognizer : public Recognizer {
 public:
  PinchRecognizer(const Vec2& screen_resolution, const Vec2& screen_size);
  std::set<GestureEventPair> update(
      const std::set<TouchPointPtr>& touch_points) override;

 protected:
  std::set<GestureEventPair> detect_pinches(
      const std::set<TouchPointPtr>& touch_points);

 protected:
  const double MIN_OPPOSING_ANGLE{0.8 * M_PI};  // in rad
};
