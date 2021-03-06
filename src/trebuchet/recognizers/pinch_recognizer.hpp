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
  std::set<TouchPointPtr> recognize(
      const std::set<TouchPointPtr>& touch_points) override;
  std::set<GestureEventPair> update() override;
  bool invalidate_touch_point(const TouchPointPtr& touch_point) override;

 protected:
  std::set<GestureEventPair> detect_pinches(
      const std::set<TouchPointPtr>& touch_points);

 protected:
  const std::chrono::milliseconds MIN_DURATION{100};
  const double MIN_OPPOSING_ANGLE{0.6 * M_PI};  // in rad

 protected:
  std::set<PinchPtr> m_pinches;
};
