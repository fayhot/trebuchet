#pragma once

#include <utility>
#include <memory>
#include <deque>

#include <touch_point.hpp>

enum class GestureState { POSSIBLE, START, END, TRIGGER };

class Gesture {
 public:
  Gesture();
  Gesture(GestureState state);
  ~Gesture() = default;

 protected:
  Gesture(Gesture&& other) noexcept;
  Gesture(Gesture const&) = delete;
  Gesture& operator=(Gesture const&) = delete;
  Gesture& operator=(Gesture&& other) noexcept;

 protected:
  GestureState m_state;
  std::deque<std::shared_ptr<TouchPoint>> m_touch_points;
};
