#include <gestures/pinch.hpp>

Pinch::Pinch(const std::deque<std::shared_ptr<TouchPoint>>& tps)
    : Gesture(tps) {}

std::ostream& Pinch::print(std::ostream& stream) const {
  stream << "Pinch <";
  print_touch_points(stream);
  return stream << ">";
}
