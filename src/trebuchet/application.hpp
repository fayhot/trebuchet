#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <map>
#include <mutex>
#include <set>
#include <vector>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <cppitertools/combinations.hpp>
#include <cppitertools/range.hpp>

#include <gestures/gesture.hpp>
#include <gestures/gesture_event.hpp>
#include <recognizers/recognizer.hpp>
#include <recognizers/tap_recognizer.hpp>
#include <recognizers/long_tap_recognizer.hpp>
#include <recognizers/fling_recognizer.hpp>
#include <recognizers/pinch_recognizer.hpp>
#include <recognizers/swipe_recognizer.hpp>
#include <touch_point.hpp>
#include <vec2.hpp>

using GestureEventPair = std::pair<std::shared_ptr<Gesture>, GestureEvent>;
using TouchPointPtr = std::shared_ptr<TouchPoint>;

class Application {
 public:
  Application(uint32_t port = 3333);
  Application(const Application&) = delete;
  ~Application() = default;

  void set_screen_resolution(const Vec2& screen_resolution);
  void set_screen_size(const Vec2& screen_size);

  const Vec2& get_screen_resolution();
  const Vec2& get_screen_size();

  void start();
  std::vector<GestureEventPair> update();

 protected:
  void start_bundle(const std::set<uint32_t>& alive_ids);
  void set_cursor(int32_t id,
                  const Vec2& pos,
                  const Vec2& velocity,
                  float acceleration);
  void end_bundle(int32_t fseq);

  void cleanup_inactive_touch_points();
  void remove_finished_gestures();
  void add_gesture_event(std::shared_ptr<Gesture> gesture, GestureEvent event);

  Vec2 tuio_to_pixels(const Vec2& pos) const;
  Vec2 tuio_to_meters(const Vec2& pos) const;

 private:
  std::unique_ptr<lo::ServerThread> m_liblo_st;

  Vec2 m_screen_resolution;
  Vec2 m_screen_size;

  std::mutex m_tp_mutex;
  std::mutex m_gestures_mutex;

  std::map<uint32_t, std::shared_ptr<TouchPoint>> m_touch_points;
  std::set<std::shared_ptr<TouchPoint>> m_unhandled_tps;

  std::vector<std::unique_ptr<Recognizer>> m_recognizers;

  std::vector<GestureEventPair> m_gesture_events;
  std::set<std::shared_ptr<Gesture>> m_active_gestures;
};
