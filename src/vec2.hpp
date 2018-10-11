#pragma once

#include <cmath>
#include <utility>

struct Vec2 {
  Vec2() = default;
  Vec2(double x, double y);

  Vec2(const Vec2 &) = default;
  Vec2(Vec2 &&other) noexcept;

  Vec2 &operator=(const Vec2 &) = default;
  Vec2 &operator=(Vec2 &&other) noexcept;

  Vec2 operator+(const Vec2 &other);
  Vec2 operator-(const Vec2 &other);
  Vec2 &operator+=(const Vec2 &other);
  Vec2 &operator-=(const Vec2 &other);

  Vec2 operator+(double s);
  Vec2 operator-(double s);
  Vec2 operator*(double s);
  Vec2 operator/(double s);

  Vec2 &operator+=(double s);
  Vec2 &operator-=(double s);
  Vec2 &operator*=(double s);
  Vec2 &operator/=(double s);

  Vec2 &normalize();
  double distance(const Vec2 &other) const;
  double length() const;

  static double dot(const Vec2 &lhs, const Vec2 &rhs);
  static double cross(const Vec2 &lhs, const Vec2 &rhs);

  double x;
  double y;
};
