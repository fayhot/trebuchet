#pragma once

#include <chrono>
#include <iostream>
#include <set>

#include <gestures/gesture.hpp>

class Pinch : public Gesture {
 public:
  Pinch(const std::set<std::shared_ptr<TouchPoint>>& first_cluster,
        const std::set<std::shared_ptr<TouchPoint>>& second_cluster);

  std::ostream& print(std::ostream& stream) const override;
};
