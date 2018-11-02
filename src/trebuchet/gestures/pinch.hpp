#pragma once

#include <chrono>
#include <cmath>
#include <memory>
#include <set>
#include <string>

#include <gestures/gesture.hpp>
#include <vec2.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;

class Pinch : public Gesture {
 public:
  Pinch(const std::set<TouchPointPtr>& first_cluster,
        const std::set<TouchPointPtr>& second_cluster);

  std::string as_string() const override;

  Vec2 first_center() const;
  Vec2 second_center() const;
  Vec2 center() const;
  double distance() const;
  bool horizontal() const;
  bool vertical() const;
  uint32_t num_fingers() const;

  const Vec2& start_center() const;
  double start_distance() const;

 protected:
  const double DIRECTION_ANGLE_THRESHOLD = 0.4 * M_PI;

 protected:
  std::set<TouchPointPtr> m_first_cluster;
  std::set<TouchPointPtr> m_second_cluster;

  Vec2 m_start_center;
  double m_start_distance;
  Vec2 m_start_direction;
  bool m_horizontal = false;
  bool m_vertical = false;
};
