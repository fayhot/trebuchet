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
#include <gestures/tap.hpp>
#include <gestures/long_tap.hpp>
#include <gestures/double_tap.hpp>
#include <gestures/pinch.hpp>
#include <gestures/fling.hpp>
#include <gestures/swipe.hpp>
#include <touch_point.hpp>
#include <vec2.hpp>

using GestureEventPair = std::pair<std::shared_ptr<Gesture>, GestureEvent>;
using TouchPointPtr = std::shared_ptr<TouchPoint>;

class GestureRecognizer {
 public:
  GestureRecognizer(uint32_t port = 3333);
  GestureRecognizer(const GestureRecognizer&) = delete;
  ~GestureRecognizer() = default;

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

  void detect_flings();
  void detect_taps();
  void detect_long_taps();
  void detect_double_taps();
  std::shared_ptr<Pinch> is_4finger_pinch(
      const std::set<TouchPointPtr>& touch_points);
  void detect_pinches();
  void detect_swipes();

  void cleanup_inactive_touch_points();
  void fire_verified_taps();
  void fire_verified_flings();
  void fire_verified_pinches();
  void remove_finished_gestures();

  bool only_unhandled_tps(
      const std::set<std::shared_ptr<TouchPoint>>& tps) const;
  void add_gesture_event(std::shared_ptr<Gesture> gesture, GestureEvent event);

  Vec2 tuio_to_pixels(const Vec2& pos) const;
  Vec2 tuio_to_meters(const Vec2& pos) const;

 protected:
  const std::chrono::milliseconds TAP_MAX_DURATION{300};
  const double TAP_MAX_DISTANCE{0.01};  // in m

  const std::chrono::milliseconds LONG_TAP_MIN_DURATION{500};

  const double DOUBLE_TAP_MAX_DISTANCE{0.02};  // in m
  const std::chrono::milliseconds DOUBLE_TAP_MAX_PAUSE{200};

  const std::array<std::array<std::size_t, 4>, 3> PINCH_INDICES{
      {{0, 1, 2, 3}, {0, 2, 1, 3}, {0, 3, 1, 2}}};
  const double PINCH_MIN_OPPOSING_ANGLE{0.9 * M_PI};  // in rad
  const std::chrono::milliseconds PINCH_MULTI_FINGER_MAX_TIME_BETWEEN{200};

  const int32_t FLING_MAX_NUM_FINGERS{3};
  const std::chrono::milliseconds FLING_MULTI_FINGER_MAX_TIME_BETWEEN{400};
  const double FLING_MIN_VELOCITY{0.5};           // in m/s
  const double FLING_MAX_ANGLE_DIFF{0.1 * M_PI};  // in rad
  const double FLING_MAX_DISTANCE{0.1};           // in m

  const double SWIPE_MIN_TRAVEL{0.01};   // in m
  const double SWIPE_MAX_VELOCITY{0.3};  // in m/s

 private:
  std::unique_ptr<lo::ServerThread> m_liblo_st;

  Vec2 m_screen_resolution;
  Vec2 m_screen_size;

  std::mutex m_tp_mutex;
  std::mutex m_gestures_mutex;

  std::map<uint32_t, std::shared_ptr<TouchPoint>> m_touch_points;

  std::set<std::shared_ptr<TouchPoint>> m_unhandled_tps;
  std::set<std::shared_ptr<Tap>> m_possible_taps;
  std::set<std::shared_ptr<Pinch>> m_possible_pinches;
  std::set<std::shared_ptr<Fling>> m_flings;
  std::set<std::shared_ptr<Gesture>> m_active_gestures;

  std::vector<GestureEventPair> m_gesture_events;
};
