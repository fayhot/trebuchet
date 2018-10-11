#pragma once

#include <chrono>
#include <utility>

#include <vec2.hpp>

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

  bool finished() const;
  double duration() const;
  double finished_since() const;

 private:
  int32_t m_id;
  Vec2 m_start_pos;
  Vec2 m_pos;
  Vec2 m_velocity;
  float m_acceleration;

  std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_update_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_end_time;
};
