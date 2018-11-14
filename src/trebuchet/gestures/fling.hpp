#pragma once

#include <memory>
#include <set>
#include <string>

#include <gestures/gesture.hpp>

class Fling : public Gesture {
 public:
  Fling(std::shared_ptr<TouchPoint> tp);
  Fling(std::set<std::shared_ptr<TouchPoint>> tps);

  bool add_touch_point(std::shared_ptr<TouchPoint> tp);

  uint32_t num_fingers() const;
  Vec2 direction() const;
  double speed() const;

  std::string as_string() const override;
};
