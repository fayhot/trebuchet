#pragma once

#include <memory>
#include <string>

#include <gestures/gesture.hpp>
#include <gestures/tap.hpp>

class LongTap : public Tap {
 public:
  LongTap(std::shared_ptr<TouchPoint> tp);

  std::string as_string() const override;
};
