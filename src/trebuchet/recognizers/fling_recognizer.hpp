#pragma once

#include <recognizers/recognizer.hpp>
#include <gestures/gesture_event.hpp>
#include <gestures/fling.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;
using GesturePtr = std::shared_ptr<Gesture>;
using FlingPtr = std::shared_ptr<Fling>;
using GestureEventPair = std::pair<GesturePtr, GestureEvent>;

class FlingRecognizer : public Recognizer {
 public:
  FlingRecognizer(const Vec2& screen_resolution, const Vec2& screen_size);
  std::set<GestureEventPair> update(
      const std::set<TouchPointPtr>& touch_points) override;
  bool invalidate_touch_point(const TouchPointPtr& touch_point) override;

 protected:
  void detect_flings(const std::set<TouchPointPtr>& touch_points);
  std::set<GestureEventPair> verified_flings();

 protected:
  const std::chrono::milliseconds MULTI_FINGER_MAX_TIME_BETWEEN{400};
  const double MIN_VELOCITY{0.5};           // in m/s
  const double MAX_ANGLE_DIFF{0.1 * M_PI};  // in rad
  const double MAX_DISTANCE{0.1};           // in m

 protected:
  std::set<FlingPtr> m_flings;
};
