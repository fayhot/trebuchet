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

double Vec2::length() const {
  return std::sqrt(x * x + y * y);
}

std::ostream& operator<<(std::ostream& stream, const Vec2& vec) {
  stream << "(" << vec.x << ", " << vec.y << ")";
  return stream;
}

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
  return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
  return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vec2 operator*(const Vec2& lhs, const Vec2& rhs) {
  return Vec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

Vec2 operator/(const Vec2& lhs, const Vec2& rhs) {
  return Vec2(lhs.x / rhs.x, lhs.y / rhs.y);
}

Vec2 operator+(const Vec2& vec, double s) {
  return Vec2(vec.x + s, vec.y + s);
}

Vec2 operator-(const Vec2& vec, double s) {
  return Vec2(vec.x - s, vec.y - s);
}

Vec2 operator*(const Vec2& vec, double s) {
  return Vec2(vec.x * s, vec.y * s);
}

Vec2 operator/(const Vec2& vec, double s) {
  return Vec2(vec.x / s, vec.y / s);
}

Vec2 operator+(double s, const Vec2& vec) {
  return vec + s;
}
Vec2 operator-(double s, const Vec2& vec) {
  return Vec2(s - vec.x, s - vec.y);
}
Vec2 operator*(double s, const Vec2& vec) {
  return vec * s;
}

Vec2 operator/(double s, const Vec2& vec) {
  return Vec2(s / vec.x, s / vec.y);
}

Vec2 normalize(const Vec2& vec) {
  return Vec2(vec).normalize();
}

double distance(const Vec2& lhs, const Vec2& rhs) {
  return Vec2(rhs - lhs).length();
}

double dot(const Vec2& lhs, const Vec2& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

double cross(const Vec2& lhs, const Vec2& rhs) {
  return (lhs.x * rhs.y) - (lhs.y * rhs.x);
}

double angle(const Vec2& lhs, const Vec2& rhs) {
  return std::acos(std::clamp(dot(normalize(lhs), normalize(rhs)), -1.0, 1.0));
}

double angle_deg(const Vec2& lhs, const Vec2& rhs) {
  return angle(lhs, rhs) * 180.0 / M_PI;
}

Vec2 centroid(const std::vector<Vec2>& vectors) {
  return std::accumulate(vectors.begin(), vectors.end(), Vec2()) /
         vectors.size();
}
