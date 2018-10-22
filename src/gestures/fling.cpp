#include <gestures/fling.hpp>

Fling::Fling(const std::set<std::shared_ptr<TouchPoint>>& tps) : Gesture(tps) {}

std::ostream& Fling::print(std::ostream& stream) const {
  stream << "Fling" << m_touch_points.size() << " <";
  print_touch_points(stream);
  return stream << ">";
}
