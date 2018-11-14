#include <recognizers/recognizer.hpp>

Recognizer::Recognizer(const Vec2& screen_resolution, const Vec2& screen_size)
    : m_screen_resolution(screen_resolution), m_screen_size(screen_size) {}

bool Recognizer::invalidate_touch_point(const TouchPointPtr& touch_point) {
  return false;
}

Vec2 Recognizer::tuio_to_pixels(const Vec2& pos) const {
  return pos * m_screen_resolution;
}

Vec2 Recognizer::tuio_to_meters(const Vec2& pos) const {
  return pos * m_screen_size;
}
