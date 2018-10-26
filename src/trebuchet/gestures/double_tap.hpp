#pragma once

#include <memory>
#include <string>

#include <gestures/gesture.hpp>

class DoubleTap : public Gesture {
 public:
  DoubleTap(std::shared_ptr<TouchPoint> first_tp,
            std::shared_ptr<TouchPoint> second_tp);

  std::string as_string() const override;
};
