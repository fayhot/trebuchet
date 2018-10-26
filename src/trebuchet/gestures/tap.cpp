#include <gestures/tap.hpp>

Tap::Tap(std::shared_ptr<TouchPoint> tp) : Gesture({tp}) {}

const std::shared_ptr<TouchPoint> Tap::touch_point() const {
  return *m_touch_points.begin();
}

std::string Tap::as_string() const {
  return "<Tap [" + touch_points_string() + "]>";
}
