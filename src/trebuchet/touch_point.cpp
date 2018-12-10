#include <touch_point.hpp>

TouchPoint::TouchPoint(int32_t id,
                       const Vec2& pos,
                       const Vec2& velocity,
                       float acceleration)
    : m_id(id),
      m_start_time(Clock::now()),
      m_update_time(m_start_time),
      m_end_time(TimePoint::min()),
      m_start_pos(pos),
      m_pos(pos),
      m_velocity(velocity),
      m_max_velocity(0.0, 0.0),
      m_acceleration(acceleration),
      m_positions({pos}),
      m_travel(0.0, 0.0) {}

TouchPoint::TouchPoint(TouchPoint&& other) noexcept {
  if (this != &other) {
    std::swap(m_id, other.m_id);
    std::swap(m_start_pos, other.m_start_pos);
    std::swap(m_pos, other.m_pos);
    std::swap(m_velocity, other.m_velocity);
    std::swap(m_max_velocity, other.m_max_velocity);
    std::swap(m_acceleration, other.m_acceleration);
    std::swap(m_positions, other.m_positions);
    std::swap(m_travel, other.m_travel);
    std::swap(m_start_time, other.m_start_time);
    std::swap(m_update_time, other.m_update_time);
    std::swap(m_end_time, other.m_end_time);
  }
}

TouchPoint& TouchPoint::operator=(TouchPoint&& other) noexcept {
  if (this != &other) {
    std::swap(m_id, other.m_id);
    std::swap(m_start_pos, other.m_start_pos);
    std::swap(m_pos, other.m_pos);
    std::swap(m_velocity, other.m_velocity);
    std::swap(m_max_velocity, other.m_max_velocity);
    std::swap(m_acceleration, other.m_acceleration);
    std::swap(m_positions, other.m_positions);
    std::swap(m_travel, other.m_travel);
    std::swap(m_start_time, other.m_start_time);
    std::swap(m_update_time, other.m_update_time);
    std::swap(m_end_time, other.m_end_time);
  }
  return *this;
}

bool TouchPoint::operator<(const TouchPoint& other) const {
  return m_id < other.id();
}

void TouchPoint::update(const Vec2& pos,
                        const Vec2& velocity,
                        float acceleration) {
  m_update_time = Clock::now();
  m_pos = pos;
  m_velocity = velocity;
  m_max_velocity = Vec2(std::max(m_max_velocity.x, m_velocity.x),
                        std::max(m_max_velocity.y, m_velocity.y));
  m_acceleration = acceleration;
  if (pos != m_positions.back()) {
    m_travel += Vec2(std::abs(m_positions.back().x - pos.x),
                     std::abs(m_positions.back().y - pos.y));
    m_positions.push_back(pos);
  }
}

void TouchPoint::end() {
  m_end_time = Clock::now();
}

uint32_t TouchPoint::id() const {
  return m_id;
}

const Vec2& TouchPoint::start_pos() const {
  return m_start_pos;
}

const Vec2& TouchPoint::pos() const {
  return m_pos;
}

const Vec2& TouchPoint::velocity() const {
  return m_velocity;
}

const Vec2& TouchPoint::max_velocity() const {
  return m_max_velocity;
}

const float TouchPoint::acceleration() const {
  return m_acceleration;
}

Vec2 TouchPoint::direction() const {
  return m_pos - m_start_pos;
}

const std::vector<Vec2>& TouchPoint::positions() const {
  return m_positions;
}

const Vec2& TouchPoint::travel() const {
  return m_travel;
}

TimePoint TouchPoint::start_time() const {
  return m_start_time;
}

TimePoint TouchPoint::update_time() const {
  return m_update_time;
}

TimePoint TouchPoint::end_time() const {
  return m_end_time;
}

bool TouchPoint::finished() const {
  return m_end_time > m_start_time;
}

std::chrono::microseconds TouchPoint::duration() const {
  if (finished()) {
    return std::chrono::duration_cast<std::chrono::microseconds>(m_end_time -
                                                                 m_start_time);
  }
  return std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() -
                                                               m_start_time);
}
