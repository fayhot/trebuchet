#pragma once

#include <iostream>

#include <gesture.hpp>
#include <gestures/tap.hpp>

class LongTap : public Tap {
 public:
  LongTap(const std::shared_ptr<TouchPoint>& tp);

  void print() const override;
};
