#pragma once

#include <chrono>
#include <utility>

#include <vec2.hpp>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

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
  const Vec2& travel() const;

  TimePoint start_time() const;
  TimePoint update_time() const;
  TimePoint end_time() const;

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
  Vec2 m_travel;

  TimePoint m_start_time;
  TimePoint m_update_time;
  TimePoint m_end_time;
};
