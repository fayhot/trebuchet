#pragma once

#include <gesture.hpp>

class Tap : public Gesture {
public:
  Tap(const std::shared_ptr<TouchPoint> &tp);

protected:
};
