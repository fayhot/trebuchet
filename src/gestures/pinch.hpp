#pragma once

#include <chrono>
#include <iostream>

#include <gesture.hpp>

class Pinch : public Gesture {
 public:
  Pinch(const std::deque<std::shared_ptr<TouchPoint>>& tps);

  void print() const override;
};
