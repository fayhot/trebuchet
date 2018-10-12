#pragma once

#include <iostream>

#include <gesture.hpp>

class DoubleTap : public Gesture {
 public:
  DoubleTap(std::shared_ptr<TouchPoint> first_tp,
            std::shared_ptr<TouchPoint> second_tp);

  void print() const override;
};
