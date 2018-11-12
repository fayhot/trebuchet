#include <gestures/pinch.hpp>

Pinch::Pinch(
    const std::set<std::pair<TouchPointPtr, TouchPointPtr>>& touch_point_pairs)
    : m_touch_point_pairs(touch_point_pairs) {
  // add all tocuh points to the gesture's set
  for (auto& tp_pair : m_touch_point_pairs) {
    m_touch_points.insert(tp_pair.first);
    m_touch_points.insert(tp_pair.second);
  }

  // compute the start center of all touch points
  std::vector<Vec2> start_positions;
  std::transform(m_touch_points.begin(), m_touch_points.end(),
                 std::back_inserter(start_positions),
                 [](auto& tp) { return tp->start_pos(); });
  m_start_center = ::centroid(start_positions);

  auto first_cluster = this->first_cluster();
  auto second_cluster = this->second_cluster();

  // compute the cluster centers
  std::vector<Vec2> first_cluster_pos;
  std::transform(first_cluster.begin(), first_cluster.end(),
                 std::back_inserter(first_cluster_pos),
                 [](auto& tp) { return tp->start_pos(); });
  auto first_center = centroid(first_cluster_pos);
  std::vector<Vec2> second_cluster_pos;
  std::transform(second_cluster.begin(), second_cluster.end(),
                 std::back_inserter(second_cluster_pos),
                 [](auto& tp) { return tp->start_pos(); });
  auto second_center = centroid(second_cluster_pos);

  // compute the cluster directions
  std::vector<Vec2> first_cluster_dir;
  std::transform(first_cluster.begin(), first_cluster.end(),
                 std::back_inserter(first_cluster_dir),
                 [](auto& tp) { return tp->direction(); });
  m_first_direction = centroid(first_cluster_dir);
  std::vector<Vec2> second_cluster_dir;
  std::transform(second_cluster.begin(), second_cluster.end(),
                 std::back_inserter(second_cluster_dir),
                 [](auto& tp) { return tp->direction(); });
  m_second_direction = centroid(second_cluster_dir);

  // compute the distance between the clusters
  m_start_distance = ::distance(first_center, second_center);

  // compute the pinch's orientation
  auto ref_vector = Vec2(0.0, 1.0);
  m_orientation = std::min(angle(ref_vector, second_center - first_center),
                           angle(ref_vector, first_center - second_center));

  auto direction = second_center - first_center;
  m_horizontal_angle = std::min(::angle(direction, Vec2(1.0, 0.0)),
                                ::angle(direction, Vec2(-1.0, 0.0)));
  m_vertical_angle = std::min(::angle(direction, Vec2(0.0, 1.0)),
                              ::angle(direction, Vec2(0.0, -1.0)));
}

std::string Pinch::as_string() const {
  std::string direction = "";
  if (horizontal()) {
    direction = "Horizontal";
  } else if (vertical()) {
    direction = "Vertical";
  }
  return "<" + direction + "Pinch [" + touch_points_string() + "]>";
}

const std::set<TouchPointPair>& Pinch::touch_point_pairs() const {
  return m_touch_point_pairs;
}

Vec2 Pinch::first_center() const {
  std::vector<Vec2> positions;
  auto first_cluster = this->first_cluster();
  std::transform(first_cluster.begin(), first_cluster.end(),
                 std::back_inserter(positions),
                 [](auto& tp) { return tp->pos(); });
  return ::centroid(positions);
}

Vec2 Pinch::second_center() const {
  std::vector<Vec2> positions;
  auto second_cluster = this->second_cluster();
  std::transform(second_cluster.begin(), second_cluster.end(),
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
  return m_horizontal_angle <= DIRECTION_ANGLE_THRESHOLD &&
         m_horizontal_angle < m_vertical_angle;
}

bool Pinch::vertical() const {
  return m_vertical_angle <= DIRECTION_ANGLE_THRESHOLD &&
         m_vertical_angle < m_horizontal_angle;
}

double Pinch::orientation() const {
  return m_orientation;
}

uint32_t Pinch::num_fingers() const {
  return m_touch_point_pairs.size();
}

const Vec2& Pinch::start_center() const {
  return m_start_center;
}

double Pinch::start_distance() const {
  return m_start_distance;
}

const std::set<TouchPointPtr> Pinch::first_cluster() const {
  std::set<TouchPointPtr> tps;
  std::transform(m_touch_point_pairs.begin(), m_touch_point_pairs.end(),
                 std::inserter(tps, tps.begin()),
                 [&](auto& tp_pair) { return tp_pair.first; });
  return tps;
}

const std::set<TouchPointPtr> Pinch::second_cluster() const {
  std::set<TouchPointPtr> tps;
  std::transform(m_touch_point_pairs.begin(), m_touch_point_pairs.end(),
                 std::inserter(tps, tps.begin()),
                 [&](auto& tp_pair) { return tp_pair.second; });
  return tps;
}
