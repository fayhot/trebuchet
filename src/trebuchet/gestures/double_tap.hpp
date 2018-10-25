#pragma once

#include <iostream>

#include <gestures/gesture.hpp>

class DoubleTap : public Gesture {
 public:
  DoubleTap(std::shared_ptr<TouchPoint> first_tp,
            std::shared_ptr<TouchPoint> second_tp);

  std::ostream& print(std::ostream& stream) const override;
};
