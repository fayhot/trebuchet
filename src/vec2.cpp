#include <vec2.hpp>

Vec2::Vec2(double x, double y) : x{x}, y{y} {}

Vec2::Vec2(Vec2&& other) noexcept {
  std::swap(x, other.x);
  std::swap(y, other.y);
}

Vec2& Vec2::operator=(Vec2&& other) noexcept {
  if (this != &other) {
    std::swap(x, other.x);
    std::swap(y, other.y);
  }
  return *this;
}

Vec2 Vec2::operator+(const Vec2& other) {
  return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) {
  return Vec2(x - other.x, y - other.y);
}

Vec2& Vec2::operator+=(const Vec2& other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

Vec2 Vec2::operator+(double s) {
  return Vec2(x + s, y + s);
}

Vec2 Vec2::operator-(double s) {
  return Vec2(x - s, y - s);
}

Vec2 Vec2::operator*(double s) {
  return Vec2(x * s, y * s);
}

Vec2 Vec2::operator/(double s) {
  return Vec2(x / s, y / s);
}

Vec2& Vec2::operator+=(double s) {
  x += s;
  y += s;
  return *this;
}

Vec2& Vec2::operator-=(double s) {
  x -= s;
  y -= s;
  return *this;
}

Vec2& Vec2::operator*=(double s) {
  x *= s;
  y *= s;
  return *this;
}

Vec2& Vec2::operator/=(double s) {
  x /= s;
  y /= s;
  return *this;
}

Vec2& Vec2::normalize() {
  if (length() == 0) {
    return *this;
  }
  *this *= (1.0 / length());
  return *this;
}

double Vec2::distance(const Vec2& other) const {
  Vec2 d(other.x - x, other.y - y);
  return d.length();
}

double Vec2::length() const {
  return std::sqrt(x * x + y * y);
}

double Vec2::dot(const Vec2& lhs, const Vec2& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

double Vec2::cross(const Vec2& lhs, const Vec2& rhs) {
  return (lhs.x * rhs.y) - (lhs.y * rhs.x);
}
