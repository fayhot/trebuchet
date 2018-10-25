#include <gestures/long_tap.hpp>

LongTap::LongTap(std::shared_ptr<TouchPoint> tp) : Tap(tp) {}

std::ostream& LongTap::print(std::ostream& stream) const {
  stream << "LongTap <";
  print_touch_points(stream);
  return stream << ">";
}
