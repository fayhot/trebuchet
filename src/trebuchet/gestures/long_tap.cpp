#include <gestures/long_tap.hpp>

LongTap::LongTap(std::shared_ptr<TouchPoint> tp) : Tap(tp) {}

std::string LongTap::as_string() const {
  return "<LongTap [" + touch_points_string() + "]>";
}
