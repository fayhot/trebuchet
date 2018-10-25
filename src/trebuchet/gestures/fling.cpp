#include <gestures/fling.hpp>

Fling::Fling(std::shared_ptr<TouchPoint> tp) : Gesture({tp}) {}

Fling::Fling(std::set<std::shared_ptr<TouchPoint>> tps) : Gesture(tps) {}

bool Fling::add_touch_point(std::shared_ptr<TouchPoint> tp) {
  if (m_touch_points.size() >= MAX_FINGERS) {
    return false;
  }
  m_touch_points.emplace(std::move(tp));
  return true;
}

uint32_t Fling::num_fingers() const {
  return m_touch_points.size();
}

Vec2 Fling::direction() const {
  return pos() - m_start_pos;
}

std::ostream& Fling::print(std::ostream& stream) const {
  stream << "Fling" << m_touch_points.size() << " <";
  print_touch_points(stream);
  return stream << ">";
}
