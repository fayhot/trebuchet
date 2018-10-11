#include <gestures/tap.hpp>

Tap::Tap(const std::shared_ptr<TouchPoint>& tp)
    : Gesture({tp}, GestureState::POSSIBLE) {}

const TouchPoint& Tap::touch_point() const {
  return *m_touch_points.front();
}

double Tap::time_finished() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now() -
             touch_point().end_time())
             .count() /
         1000.0;
}
