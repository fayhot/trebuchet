#pragma once

#include <iostream>

#include <gestures/gesture.hpp>

class Fling : public Gesture {
 public:
  Fling(std::shared_ptr<TouchPoint> tp);
  Fling(std::set<std::shared_ptr<TouchPoint>> tps);

  std::ostream& print(std::ostream& stream) const override;
};
