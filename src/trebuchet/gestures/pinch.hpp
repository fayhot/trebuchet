#pragma once

#include <chrono>
#include <cmath>
#include <memory>
#include <set>
#include <string>

#include <gestures/gesture.hpp>
#include <vec2.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;
using TouchPointPair = std::pair<TouchPointPtr, TouchPointPtr>;

class Pinch : public Gesture {
 public:
  Pinch(const std::set<std::pair<TouchPointPtr, TouchPointPtr>>&
            touch_point_pairs);

  std::string as_string() const override;

  const std::set<TouchPointPair>& touch_point_pairs() const;

  Vec2 first_center() const;
  Vec2 second_center() const;
  Vec2 center() const;
  double distance() const;
  bool horizontal() const;
  bool vertical() const;
  double orientation() const;
  uint32_t num_fingers() const;

  const Vec2& start_center() const;
  double start_distance() const;

 protected:
  const std::set<TouchPointPtr> first_cluster() const;
  const std::set<TouchPointPtr> second_cluster() const;

 protected:
  const double DIRECTION_ANGLE_THRESHOLD = 0.2 * M_PI;

 protected:
  std::set<TouchPointPair> m_touch_point_pairs;

  Vec2 m_first_direction;
  Vec2 m_second_direction;

  Vec2 m_start_center;
  double m_start_distance;
  double m_orientation;
  double m_horizontal_angle;
  double m_vertical_angle;
};
