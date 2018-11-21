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

  Vec2 first_pos() const;
  Vec2 second_pos() const;
  Vec2 center() const;
  double distance() const;
  bool horizontal() const;
  bool vertical() const;
  double orientation() const;
  uint32_t num_fingers() const;

  const Vec2& start_center() const;
  const Vec2& first_start_pos() const;
  const Vec2& second_start_pos() const;
  double start_distance() const;

 protected:
  const std::set<TouchPointPtr> first_cluster() const;
  const std::set<TouchPointPtr> second_cluster() const;

 protected:
  const double DIRECTION_ANGLE_THRESHOLD = 0.2 * M_PI;

 protected:
  std::set<TouchPointPair> m_touch_point_pairs;

  Vec2 m_direction_first;
  Vec2 m_direction_second;

  Vec2 m_start_center;
  Vec2 m_first_start_pos;
  Vec2 m_second_start_pos;
  double m_start_distance;

  double m_orientation;
  double m_horizontal_angle;
  double m_vertical_angle;
};
