#include <gestures/swipe.hpp>

Swipe::Swipe(std::shared_ptr<TouchPoint> tp) : Gesture({tp}) {}

const std::vector<Vec2>& Swipe::positions() const {
  return (*m_touch_points.begin())->positions();
}

std::ostream& Swipe::print(std::ostream& stream) const {
  stream << "Swipe <";
  print_touch_points(stream);
  return stream << ">";
}
