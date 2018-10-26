#include <gestures/pinch.hpp>

Pinch::Pinch(std::set<std::shared_ptr<TouchPoint>> first_cluster,
             std::set<std::shared_ptr<TouchPoint>> second_cluster)
    : m_first_cluster(first_cluster), m_second_cluster(second_cluster) {
  m_touch_points.insert(first_cluster.begin(), first_cluster.end());
  m_touch_points.insert(second_cluster.begin(), second_cluster.end());

  m_start_center = center();
  m_start_distance = distance();
  m_start_direction = direction();

  m_horizontal =
      (::angle(m_start_direction, Vec2(1.0, 0.0)) <
       DIRECTION_ANGLE_THRESHOLD) ||
      (::angle(m_start_direction, Vec2(-1.0, 0.0)) < DIRECTION_ANGLE_THRESHOLD);
  if (!m_horizontal) {
    m_vertical = (::angle(m_start_direction, Vec2(0.0, 1.0)) <
                  DIRECTION_ANGLE_THRESHOLD) ||
                 (::angle(m_start_direction, Vec2(0.0, -1.0)) <
                  DIRECTION_ANGLE_THRESHOLD);
  }
}

std::string Pinch::as_string() const {
  std::string direction = "";
  if (m_horizontal) {
    direction = "Horizontal";
  } else if (m_vertical) {
    direction = "Vertical";
  }
  return "<" + direction + "Pinch [" + touch_points_string() + "]>";
}

Vec2 Pinch::first_center() const {
  std::vector<Vec2> positions;
  std::transform(m_first_cluster.begin(), m_first_cluster.end(),
                 std::back_inserter(positions),
                 [](auto& tp) { return tp->pos(); });
  return centroid(positions);
}

Vec2 Pinch::second_center() const {
  std::vector<Vec2> positions;
  std::transform(m_second_cluster.begin(), m_second_cluster.end(),
                 std::back_inserter(positions),
                 [](auto& tp) { return tp->pos(); });
  return centroid(positions);
}

Vec2 Pinch::center() const {
  return centroid({first_center(), second_center()});
}

double Pinch::distance() const {
  return ::distance(first_center(), second_center());
}

Vec2 Pinch::direction() const {
  return second_center() - first_center();
}

bool Pinch::horizontal() const {
  return m_horizontal;
}

bool Pinch::vertical() const {
  return m_vertical;
}

uint32_t Pinch::num_fingers() const {
  return m_touch_points.size() / 2;
}

const Vec2& Pinch::start_center() const {
  return m_start_center;
}

double Pinch::start_distance() const {
  return m_start_distance;
}

const Vec2& Pinch::start_direction() const {
  return m_start_direction;
}
