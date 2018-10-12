#pragma once

#include <chrono>

#include <gesture.hpp>
#include <gestures/tap.hpp>

class LongTap : public Tap {
 public:
  LongTap(const std::shared_ptr<TouchPoint>& tp);
};
