#include <gesture.hpp>

Gesture::Gesture() : m_state(GestureState::START) {}

Gesture::Gesture(GestureState state) : m_state(state) {}

Gesture::Gesture(Gesture&& other) noexcept {
  std::swap(m_touch_points, other.m_touch_points);
}

Gesture& Gesture::operator=(Gesture&& other) noexcept {
  if (this != &other) {
    std::swap(m_state, other.m_state);
    std::swap(m_touch_points, other.m_touch_points);
  }
  return *this;
}
