#pragma once

#include <recognizers/recognizer.hpp>
#include <gestures/gesture_event.hpp>
#include <gestures/swipe.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;
using GesturePtr = std::shared_ptr<Gesture>;
using SwipePtr = std::shared_ptr<Swipe>;
using GestureEventPair = std::pair<GesturePtr, GestureEvent>;

class SwipeRecognizer : public Recognizer {
 public:
  SwipeRecognizer(const Vec2& screen_resolution, const Vec2& screen_size);
  std::set<TouchPointPtr> recognize(const std::set<TouchPointPtr>& touch_points) override;
  std::set<GestureEventPair> update() override;
  bool invalidate_touch_point(const TouchPointPtr& touch_point) override;

 protected:
  void detect_swipes(const std::set<TouchPointPtr>& touch_points);
  std::set<GestureEventPair> verified_swipes();

 protected:
  const std::chrono::milliseconds MIN_DURATION{500};
  const double MIN_TRAVEL{0.02};   // in m
  const double MAX_VELOCITY{0.3};  // in m/s

 protected:
  std::set<SwipePtr> m_possible_swipes;
  std::set<TouchPointPtr> m_possible_swipes_tps;
};
