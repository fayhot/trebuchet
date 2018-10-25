#pragma once

#include <cmath>
#include <limits>

inline bool almost_equal_relative(double lhs, double rhs) {
  double diff = std::fabs(lhs - rhs);
  lhs = std::fabs(lhs);
  rhs = std::fabs(rhs);
  double largest = (rhs > lhs) ? rhs : lhs;
  if (diff <= largest * std::numeric_limits<double>::epsilon()) {
    return true;
  }
  return false;
}
