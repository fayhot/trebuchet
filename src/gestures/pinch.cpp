#include <gestures/pinch.hpp>

Pinch::Pinch(const std::set<std::shared_ptr<TouchPoint>>& first_cluster,
             const std::set<std::shared_ptr<TouchPoint>>& second_cluster) {
  m_touch_points.insert(m_touch_points.end(), first_cluster.begin(),
                        first_cluster.end());
  m_touch_points.insert(m_touch_points.end(), second_cluster.begin(),
                        second_cluster.end());
}

std::ostream& Pinch::print(std::ostream& stream) const {
  stream << "Pinch <";
  print_touch_points(stream);
  return stream << ">";
}
