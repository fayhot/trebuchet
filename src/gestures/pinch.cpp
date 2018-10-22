#include <gestures/pinch.hpp>

Pinch::Pinch(const std::deque<std::shared_ptr<TouchPoint>>& tps)
    : Gesture(tps, GestureState::START) {}

void Pinch::print() const {
  std::cout << "Pinch";
}
