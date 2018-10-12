#pragma once

#include <deque>

#include <gesture.hpp>

class DoubleTap : public Gesture {
 public:
  DoubleTap(std::shared_ptr<TouchPoint> first_tp,
            std::shared_ptr<TouchPoint> second_tp);
};
