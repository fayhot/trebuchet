#pragma once

#include <algorithm>
#include <iostream>
#include <chrono>
#include <utility>
#include <memory>
#include <deque>

#include <touch_point.hpp>

class Gesture {
 public:
  Gesture(const std::deque<std::shared_ptr<TouchPoint>>& tps);
  ~Gesture() = default;

  const std::deque<std::shared_ptr<TouchPoint>> touch_points() const;

  virtual bool finished() const;
  virtual double time_finished() const;

  std::ostream& print_touch_points(std::ostream& stream) const;
  virtual std::ostream& print(std::ostream& stream) const;

 protected:
  Gesture(Gesture&& other) noexcept;
  Gesture(Gesture const&) = delete;
  Gesture& operator=(Gesture const&) = delete;
  Gesture& operator=(Gesture&& other) noexcept;

 protected:
  std::deque<std::shared_ptr<TouchPoint>> m_touch_points;
};
