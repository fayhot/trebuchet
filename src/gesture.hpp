#pragma once

#include <algorithm>
#include <iostream>
#include <chrono>
#include <utility>
#include <memory>
#include <deque>

#include <touch_point.hpp>

enum class GestureState { POSSIBLE, START, END, TRIGGER };

class Gesture {
 public:
  Gesture(GestureState state = GestureState::START);
  Gesture(const std::deque<std::shared_ptr<TouchPoint>>& tps,
          GestureState state = GestureState::START);
  ~Gesture() = default;

  const std::deque<std::shared_ptr<TouchPoint>> touch_points() const;

  GestureState state() const;
  virtual bool finished() const;
  virtual double time_finished() const;

  void set_state(GestureState state);

  virtual void print() const;

 protected:
  Gesture(Gesture&& other) noexcept;
  Gesture(Gesture const&) = delete;
  Gesture& operator=(Gesture const&) = delete;
  Gesture& operator=(Gesture&& other) noexcept;

 protected:
  GestureState m_state;
  std::deque<std::shared_ptr<TouchPoint>> m_touch_points;
};
