#pragma once

#include <memory>
#include <set>

#include <gestures/gesture.hpp>
#include <gestures/gesture_event.hpp>
#include <touch_point.hpp>
#include <vec2.hpp>

using TouchPointPtr = std::shared_ptr<TouchPoint>;
using GesturePtr = std::shared_ptr<Gesture>;
using GestureEventPair = std::pair<GesturePtr, GestureEvent>;

class Recognizer {
 public:
  Recognizer(const Vec2& screen_resolution, const Vec2& screen_size);

  virtual std::set<TouchPointPtr> recognize(const std::set<TouchPointPtr>& touch_points) = 0;
  virtual std::set<GestureEventPair> update() = 0;
  virtual bool invalidate_touch_point(const TouchPointPtr& touch_point);

 protected:
  Vec2 tuio_to_pixels(const Vec2& pos) const;
  Vec2 tuio_to_meters(const Vec2& pos) const;

 protected:
  Vec2 m_screen_resolution;
  Vec2 m_screen_size;
};
