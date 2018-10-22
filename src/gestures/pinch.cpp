#include <gestures/pinch.hpp>

Pinch::Pinch(const std::set<std::shared_ptr<TouchPoint>>& first_cluster,
             const std::set<std::shared_ptr<TouchPoint>>& second_cluster)
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

std::ostream& Pinch::print(std::ostream& stream) const {
  if (m_horizontal) {
    stream << "Horizontal";
  } else if (m_vertical) {
    stream << "Vertical";
  }
  stream << "Pinch" << m_touch_points.size() / 2 << " <";
  print_touch_points(stream);
  return stream << ">";
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
