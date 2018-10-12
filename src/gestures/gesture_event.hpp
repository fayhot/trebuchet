#pragma once

#include <gesture.hpp>

struct GestureEvent {
  GestureEvent(std::shared_ptr<Gesture> gesture,
               GestureState state = GestureState::START)
      : gesture(gesture), state(state) {}

  std::shared_ptr<Gesture> gesture;
  GestureState state;
};
