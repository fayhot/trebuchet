#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include <utils.hpp>

struct Vec2 {
  Vec2();
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
  Vec2& abs();

  double x;
  double y;
};

std::ostream& operator<<(std::ostream& stream, const Vec2& vec);

bool operator==(const Vec2& lhs, const Vec2& rhs);
bool operator!=(const Vec2& lhs, const Vec2& rhs);
bool operator<(const Vec2& lhs, const Vec2& rhs);
bool operator>(const Vec2& lhs, const Vec2& rhs);
bool operator<=(const Vec2& lhs, const Vec2& rhs);
bool operator>=(const Vec2& lhs, const Vec2& rhs);

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

Vec2 abs(const Vec2& vec);
Vec2 normalize(const Vec2& vec);
double distance(const Vec2& lhs, const Vec2& rhs);
double dot(const Vec2& lhs, const Vec2& rhs);
double cross(const Vec2& lhs, const Vec2& rhs);
double angle(const Vec2& lhs, const Vec2& rhs);
double angle_deg(const Vec2& lhs, const Vec2& rhs);
double angle_horizontal(const Vec2& vec);
double angle_vertical(const Vec2& vec);
Vec2 flip_x(const Vec2& vec);
Vec2 flip_y(const Vec2& vec);
Vec2 flip(const Vec2& vec);
Vec2 centroid(const std::vector<Vec2>& vectors);
