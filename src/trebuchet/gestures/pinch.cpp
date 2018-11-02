#include <gestures/pinch.hpp>

Pinch::Pinch(const std::set<TouchPointPtr>& first_cluster,
             const std::set<TouchPointPtr>& second_cluster)
    : m_first_cluster(first_cluster), m_second_cluster(second_cluster) {
  // add all tocuh points to the gesture's set
  for (auto& tp : m_first_cluster) {
    m_touch_points.insert(tp);
  }
  for (auto& tp : m_second_cluster) {
    m_touch_points.insert(tp);
  }

  // compute the start center of all touch points
  std::vector<Vec2> start_positions;
  std::transform(m_touch_points.begin(), m_touch_points.end(),
                 std::back_inserter(start_positions),
                 [](auto& tp) { return tp->start_pos(); });
  m_start_center = ::centroid(start_positions);

  // compute the cluster centers
  std::vector<Vec2> first_cluster_pos;
  std::transform(m_first_cluster.begin(), m_first_cluster.end(),
                 std::back_inserter(first_cluster_pos),
                 [](auto& tp) { return tp->start_pos(); });
  auto first_center = centroid(first_cluster_pos);
  std::vector<Vec2> second_cluster_pos;
  std::transform(m_second_cluster.begin(), m_second_cluster.end(),
                 std::back_inserter(second_cluster_pos),
                 [](auto& tp) { return tp->start_pos(); });
  auto second_center = centroid(second_cluster_pos);

  // compute the distance between the clusters
  m_start_distance = ::distance(first_center, second_center);

  // compute the pinch's orientation
  auto direction = second_center - first_center;
  if (std::min(::angle(direction, Vec2(1.0, 0.0)),
               ::angle(direction, Vec2(-1.0, 0.0))) <=
      DIRECTION_ANGLE_THRESHOLD) {
    m_horizontal = true;
  } else if (std::min(::angle(direction, Vec2(0.0, 1.0)),
                      ::angle(direction, Vec2(0.0, -1.0))) <=
             DIRECTION_ANGLE_THRESHOLD) {
    m_vertical = true;
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
  return ::centroid(positions);
}

Vec2 Pinch::second_center() const {
  std::vector<Vec2> positions;
  std::transform(m_second_cluster.begin(), m_second_cluster.end(),
                 std::back_inserter(positions),
                 [](auto& tp) { return tp->pos(); });
  return ::centroid(positions);
}

Vec2 Pinch::center() const {
  return ::centroid({first_center(), second_center()});
}

double Pinch::distance() const {
  return ::distance(first_center(), second_center());
}

bool Pinch::horizontal() const {
  return m_horizontal;
}

bool Pinch::vertical() const {
  return m_vertical;
}

uint32_t Pinch::num_fingers() const {
  return m_first_cluster.size();
}

const Vec2& Pinch::start_center() const {
  return m_start_center;
}

double Pinch::start_distance() const {
  return m_start_distance;
}
