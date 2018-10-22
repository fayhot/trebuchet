#include <gestures/double_tap.hpp>

DoubleTap::DoubleTap(std::shared_ptr<TouchPoint> first_tp,
                     std::shared_ptr<TouchPoint> second_tp)
    : Gesture({first_tp, second_tp}) {}

std::ostream& DoubleTap::print(std::ostream& stream) const {
  stream << "DoubleTap <";
  print_touch_points(stream);
  return stream << ">";
}
