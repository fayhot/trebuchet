#pragma once

#include <iostream>

#include <gestures/gesture.hpp>

class Fling : public Gesture {
 public:
  Fling(std::shared_ptr<TouchPoint> tp);
  Fling(std::set<std::shared_ptr<TouchPoint>> tps);

  bool add_touch_point(std::shared_ptr<TouchPoint> tp);

  uint32_t num_fingers() const;
  Vec2 direction() const;

  std::ostream& print(std::ostream& stream) const override;

 protected:
  static const uint32_t MAX_FINGERS = 4;
};
