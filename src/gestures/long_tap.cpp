#include <gestures/long_tap.hpp>

LongTap::LongTap(const std::shared_ptr<TouchPoint>& tp)
    : Tap(tp, GestureState::START) {}

void LongTap::print() const {
  std::cout << "LongTap";
}
