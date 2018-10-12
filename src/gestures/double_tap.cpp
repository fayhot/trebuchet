#include <gestures/double_tap.hpp>

DoubleTap::DoubleTap(std::shared_ptr<TouchPoint> first_tp,
                     std::shared_ptr<TouchPoint> second_tp)
    : Gesture({first_tp, second_tp}) {}

void DoubleTap::print() const {
  std::cout << "DoubleTap";
}
