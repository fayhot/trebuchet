#pragma once

#include <algorithm>
#include <iostream>
#include <chrono>
#include <utility>
#include <memory>
#include <set>
#include <vector>

#include <touch_point.hpp>

class Gesture {
 public:
  Gesture() = default;
  Gesture(std::set<std::shared_ptr<TouchPoint>> tps);
  ~Gesture() = default;

  const std::set<std::shared_ptr<TouchPoint>> touch_points() const;

  const Vec2& start_pos() const;
  Vec2 pos() const;
  Vec2 velocity() const;

  virtual bool finished() const;
  virtual double time_finished() const;
  virtual double age() const;

  std::ostream& print_touch_points(std::ostream& stream) const;
  virtual std::ostream& print(std::ostream& stream) const;

 protected:
  Gesture(Gesture&& other) noexcept;
  Gesture(Gesture const&) = delete;
  Gesture& operator=(Gesture const&) = delete;
  Gesture& operator=(Gesture&& other) noexcept;

 protected:
  std::set<std::shared_ptr<TouchPoint>> m_touch_points;
  Vec2 m_start_pos;
};
