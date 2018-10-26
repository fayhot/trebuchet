#include <gestures/swipe.hpp>

Swipe::Swipe(std::shared_ptr<TouchPoint> tp) : Gesture({tp}) {}

const std::vector<Vec2>& Swipe::positions() const {
  return (*m_touch_points.begin())->positions();
}

std::string Swipe::as_string() const {
  return "<Swipe " + touch_points_string() + "]>";
}
