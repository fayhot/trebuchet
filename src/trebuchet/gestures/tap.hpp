#pragma once

#include <memory>
#include <string>

#include <gestures/gesture.hpp>

class Tap : public Gesture {
 public:
  Tap(std::shared_ptr<TouchPoint> tp);

  const std::shared_ptr<TouchPoint> touch_point() const;

  double time_finished() const override;

  std::string as_string() const override;
};
