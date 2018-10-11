#pragma once

#include <chrono>
#include <utility>

#include <TuioCursor.h>

#include <vec2.hpp>

class TouchPoint {
 public:
  TouchPoint() = default;
  TouchPoint(TUIO::TuioCursor* tcur);
  TouchPoint(const TouchPoint& other) = delete;
  TouchPoint(TouchPoint&& other) noexcept;
  ~TouchPoint() = default;

  TouchPoint& operator=(const TouchPoint& other) = delete;
  TouchPoint& operator=(TouchPoint&& other) noexcept;

  void update(TUIO::TuioCursor* tcur);
  void end();

  uint32_t id() const;
  const Vec2& start_pos() const;
  const Vec2& pos() const;

  bool finished() const;
  double duration() const;
  double finished_since() const;

 private:
  uint32_t m_id;

  std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_update_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_end_time;

  Vec2 m_start_pos;
  Vec2 m_pos;
};
