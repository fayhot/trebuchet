#include <touch_point.hpp>

TouchPoint::TouchPoint(int32_t id,
                       const Vec2& pos,
                       const Vec2& velocity,
                       float acceleration)
    : m_id(id),
      m_start_time(clock_type::now()),
      m_update_time(m_start_time),
      m_end_time(time_point::min()),
      m_start_pos(pos),
      m_pos(pos),
      m_velocity(velocity),
      m_acceleration(acceleration) {}

TouchPoint::TouchPoint(TouchPoint&& other) noexcept {
  if (this != &other) {
    std::swap(m_id, other.m_id);
    std::swap(m_start_pos, other.m_start_pos);
    std::swap(m_pos, other.m_pos);
    std::swap(m_velocity, other.m_velocity);
    std::swap(m_acceleration, other.m_acceleration);
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
    std::swap(m_acceleration, other.m_acceleration);
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
  m_update_time = clock_type::now();
  m_pos = pos;
  m_velocity = velocity;
  m_acceleration = acceleration;
}

void TouchPoint::end() {
  m_end_time = clock_type::now();
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

const float TouchPoint::acceleration() const {
  return m_acceleration;
}

Vec2 TouchPoint::direction() const {
  return m_pos - m_start_pos;
}

time_point TouchPoint::start_time() const {
  return m_start_time;
}

time_point TouchPoint::update_time() const {
  return m_update_time;
}

time_point TouchPoint::end_time() const {
  return m_end_time;
}

bool TouchPoint::finished() const {
  return m_end_time != time_point::min();
}

double TouchPoint::duration() const {
  auto end = m_end_time;
  if (!finished()) {
    end = clock_type::now();
  }
  return std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                               m_start_time)
             .count() /
         1000.0;
}

double TouchPoint::finished_since() const {
  if (finished()) {
    auto now = clock_type::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                 m_end_time)
               .count() /
           1000.0;
  }
  return -1.0;
}
