#include <gestures/gesture.hpp>

Gesture::Gesture() : m_start_time(Clock::now()) {}

Gesture::Gesture(std::set<std::shared_ptr<TouchPoint>> tps)
    : m_touch_points(tps), m_end_time(TimePoint::min()) {
  m_start_pos = pos();

  auto start_times = std::vector<TimePoint>();
  std::transform(tps.begin(), tps.end(), std::back_inserter(start_times),
                 [&](auto& tp) { return tp->start_time(); });
  m_start_time = *std::min_element(start_times.begin(), start_times.end());

  update();
}

Gesture::Gesture(Gesture&& other) noexcept {
  std::swap(m_touch_points, other.m_touch_points);
  std::swap(m_start_pos, other.m_start_pos);
  std::swap(m_start_time, other.m_start_time);
  std::swap(m_end_time, other.m_end_time);
}

Gesture& Gesture::operator=(Gesture&& other) noexcept {
  if (this != &other) {
    std::swap(m_touch_points, other.m_touch_points);
    std::swap(m_start_pos, other.m_start_pos);
    std::swap(m_start_time, other.m_start_time);
    std::swap(m_end_time, other.m_end_time);
  }
  return *this;
}

const std::set<std::shared_ptr<TouchPoint>> Gesture::touch_points() const {
  return m_touch_points;
}

void Gesture::update() {
  if (!finished()) {
    auto end_times = std::vector<TimePoint>();
    std::transform(m_touch_points.begin(), m_touch_points.end(),
                   std::back_inserter(end_times),
                   [&](auto& tp) { return tp->end_time(); });
    auto max_end_time = *std::max_element(end_times.begin(), end_times.end());
    if (max_end_time > m_start_time) {
      m_end_time = max_end_time;
    }
  }
}

const Vec2& Gesture::start_pos() const {
  return m_start_pos;
}

Vec2 Gesture::pos() const {
  auto positions = std::vector<Vec2>();
  std::transform(m_touch_points.begin(), m_touch_points.end(),
                 std::back_inserter(positions),
                 [&](auto& tp) { return tp->pos(); });
  return centroid(positions);
}

Vec2 Gesture::velocity() const {
  auto velocities = std::vector<Vec2>();
  std::transform(m_touch_points.begin(), m_touch_points.end(),
                 std::back_inserter(velocities),
                 [&](auto& tp) { return tp->velocity(); });
  return centroid(velocities);
}

TimePoint Gesture::start_time() const {
  return m_start_time;
}

TimePoint Gesture::end_time() const {
  return m_end_time;
}

std::chrono::microseconds Gesture::duration() const {
  if (finished()) {
    return std::chrono::duration_cast<std::chrono::microseconds>(m_end_time -
                                                                 m_start_time);
  }
  return std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() -
                                                               m_start_time);
}

bool Gesture::finished() const {
  return m_end_time > m_start_time;
}

std::string Gesture::touch_points_string() const {
  auto it = m_touch_points.begin();
  auto out = std::to_string((*it++)->id());
  while (it != m_touch_points.end()) {
    out += " " + std::to_string((*it++)->id());
  }
  return out;
}

std::string Gesture::as_string() const {
  return "<Gesture [" + touch_points_string() + "]>";
}
