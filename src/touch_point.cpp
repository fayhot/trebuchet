#include <touch_point.hpp>

TouchPoint::TouchPoint(TUIO::TuioCursor* tcur)
    : m_id(tcur->getSessionID()),
      m_start_time(std::chrono::high_resolution_clock::now()),
      m_update_time(m_start_time),
      m_end_time(
          std::chrono::time_point<std::chrono::high_resolution_clock>::min()),
      m_start_pos(tcur->getX(), tcur->getY()),
      m_pos(m_start_pos) {}

TouchPoint::TouchPoint(TouchPoint&& other) noexcept {
  if (this != &other) {
    std::swap(m_id, other.m_id);
    std::swap(m_start_time, other.m_start_time);
    std::swap(m_update_time, other.m_update_time);
    std::swap(m_end_time, other.m_end_time);
    std::swap(m_start_pos, other.m_start_pos);
    std::swap(m_pos, other.m_pos);
  }
}

TouchPoint& TouchPoint::operator=(TouchPoint&& other) noexcept {
  if (this != &other) {
    std::swap(m_id, other.m_id);
    std::swap(m_start_time, other.m_start_time);
    std::swap(m_update_time, other.m_update_time);
    std::swap(m_end_time, other.m_end_time);
    std::swap(m_start_pos, other.m_start_pos);
    std::swap(m_pos, other.m_pos);
  }
  return *this;
}

void TouchPoint::update(TUIO::TuioCursor* tcur) {
  if (tcur->getSessionID() == m_id) {
    m_update_time = std::chrono::high_resolution_clock::now();
    m_pos = Vec2(tcur->getX(), tcur->getY());
  }
}

void TouchPoint::end() {
  m_end_time = std::chrono::high_resolution_clock::now();
}
