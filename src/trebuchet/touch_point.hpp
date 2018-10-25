#pragma once

#include <chrono>
#include <utility>

#include <vec2.hpp>

using clock_type = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clock_type>;

class TouchPoint {
 public:
  TouchPoint(int32_t id,
             const Vec2& pos,
             const Vec2& velocity,
             float acceleration);
  TouchPoint(const TouchPoint& other) = delete;
  TouchPoint(TouchPoint&& other) noexcept;
  ~TouchPoint() = default;

  TouchPoint& operator=(const TouchPoint& other) = delete;
  TouchPoint& operator=(TouchPoint&& other) noexcept;

  bool operator<(const TouchPoint& other) const;

  void update(const Vec2& pos, const Vec2& velocity, float acceleration);
  void end();

  uint32_t id() const;
  const Vec2& start_pos() const;
  const Vec2& pos() const;
  const Vec2& velocity() const;
  const float acceleration() const;
  Vec2 direction() const;
  const std::vector<Vec2>& positions() const;
  Vec2 travel() const;

  time_point start_time() const;
  time_point update_time() const;
  time_point end_time() const;

  bool finished() const;
  double age() const;
  double duration() const;
  double finished_since() const;

 private:
  int32_t m_id;
  Vec2 m_start_pos;
  Vec2 m_pos;
  Vec2 m_velocity;
  float m_acceleration;

  std::vector<Vec2> m_positions;

  time_point m_start_time;
  time_point m_update_time;
  time_point m_end_time;
};
