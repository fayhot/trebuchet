#pragma once

#include <iostream>

#include <gestures/gesture.hpp>

class Swipe : public Gesture {
 public:
  Swipe(std::shared_ptr<TouchPoint> tp);

  const std::vector<Vec2>& positions() const;

  std::ostream& print(std::ostream& stream) const override;
};
