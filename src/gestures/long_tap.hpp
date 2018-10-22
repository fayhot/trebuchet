#pragma once

#include <iostream>

#include <gesture.hpp>
#include <gestures/tap.hpp>

class LongTap : public Tap {
 public:
  LongTap(const std::shared_ptr<TouchPoint>& tp);

  std::ostream& print(std::ostream& stream) const override;
};
