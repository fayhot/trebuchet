#include <gestures/tap.hpp>

Tap::Tap(std::shared_ptr<TouchPoint> tp) : Gesture({tp}) {}

const std::shared_ptr<TouchPoint> Tap::touch_point() const {
  return *m_touch_points.begin();
}

double Tap::time_finished() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             clock_type::now() - touch_point()->end_time())
             .count() /
         1000.0;
}

std::ostream& Tap::print(std::ostream& stream) const {
  stream << "Tap <";
  print_touch_points(stream);
  return stream << ">";
}
