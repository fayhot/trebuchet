#include <gestures/tap.hpp>

Tap::Tap(const std::shared_ptr<TouchPoint>& tp) : Gesture() {
  m_touch_points.push_back(tp);
}
