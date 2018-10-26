#pragma once

#include <memory>
#include <string>

#include <gestures/gesture.hpp>

class Swipe : public Gesture {
 public:
  Swipe(std::shared_ptr<TouchPoint> tp);

  const std::vector<Vec2>& positions() const;

  std::string as_string() const override;
};
