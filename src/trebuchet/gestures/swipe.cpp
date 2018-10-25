#include <gestures/swipe.hpp>

Swipe::Swipe(std::shared_ptr<TouchPoint> tp) : Gesture({tp}) {}

std::ostream& Swipe::print(std::ostream& stream) const {
  stream << "Swipe <";
  print_touch_points(stream);
  return stream << ">";
}
