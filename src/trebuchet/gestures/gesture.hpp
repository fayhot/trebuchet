#pragma once

#include <algorithm>
#include <chrono>
#include <utility>
#include <memory>
#include <set>
#include <vector>

#include <touch_point.hpp>

class Gesture {
 public:
  Gesture();
  Gesture(std::set<std::shared_ptr<TouchPoint>> tps);
  ~Gesture() = default;

  const std::set<std::shared_ptr<TouchPoint>> touch_points() const;

  virtual void update();

  const Vec2& start_pos() const;
  Vec2 pos() const;
  Vec2 velocity() const;
  TimePoint start_time() const;
  TimePoint end_time() const;
  std::chrono::microseconds duration() const;

  virtual bool finished() const;

  std::string touch_points_string() const;
  virtual std::string as_string() const;

 protected:
  Gesture(Gesture&& other) noexcept;
  Gesture(Gesture const&) = delete;
  Gesture& operator=(Gesture const&) = delete;
  Gesture& operator=(Gesture&& other) noexcept;

 protected:
  std::set<std::shared_ptr<TouchPoint>> m_touch_points;
  Vec2 m_start_pos;
  TimePoint m_start_time;
  TimePoint m_end_time;
};
