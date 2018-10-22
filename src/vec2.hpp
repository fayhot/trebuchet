#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>

struct Vec2 {
  Vec2() = default;
  Vec2(double x, double y);
  ~Vec2() = default;

  Vec2(const Vec2&) = default;
  Vec2(Vec2&& other) noexcept;

  Vec2& operator=(const Vec2&) = default;
  Vec2& operator=(Vec2&& other) noexcept;

  Vec2& operator+=(const Vec2& other);
  Vec2& operator-=(const Vec2& other);

  Vec2& operator+=(double s);
  Vec2& operator-=(double s);
  Vec2& operator*=(double s);
  Vec2& operator/=(double s);

  Vec2& normalize();
  double length() const;

  double x;
  double y;
};

std::ostream& operator<<(std::ostream& stream, const Vec2& vec);

Vec2 operator+(const Vec2& lhs, const Vec2& rhs);
Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
Vec2 operator*(const Vec2& lhs, const Vec2& rhs);
Vec2 operator/(const Vec2& lhs, const Vec2& rhs);

Vec2 operator+(const Vec2& vec, double s);
Vec2 operator-(const Vec2& vec, double s);
Vec2 operator*(const Vec2& vec, double s);
Vec2 operator/(const Vec2& vec, double s);
Vec2 operator+(double s, const Vec2& vec);
Vec2 operator-(double s, const Vec2& vec);
Vec2 operator*(double s, const Vec2& vec);
Vec2 operator/(double s, const Vec2& vec);

Vec2 normalize(const Vec2& vec);
double distance(const Vec2& lhs, const Vec2& rhs);
double dot(const Vec2& lhs, const Vec2& rhs);
double cross(const Vec2& lhs, const Vec2& rhs);
double angle(const Vec2& lhs, const Vec2& rhs);
double angle_deg(const Vec2& lhs, const Vec2& rhs);
