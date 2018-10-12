#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <set>
#include <deque>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <gesture.hpp>
#include <gestures/tap.hpp>
#include <gestures/long_tap.hpp>
#include <gestures/double_tap.hpp>
#include <touch_point.hpp>
#include <vec2.hpp>

class GestureRecognizer {
 public:
  GestureRecognizer(uint32_t port = 3333);
  GestureRecognizer(const GestureRecognizer&) = delete;
  ~GestureRecognizer() = default;

  void start();
  void update();

 protected:
  void start_bundle(const std::set<uint32_t>& alive_ids);
  void set_cursor(int32_t id,
                  const Vec2& pos,
                  const Vec2& velocity,
                  float acceleration);
  void end_bundle(int32_t fseq);

  void detect_taps();
  void detect_long_taps();
  void detect_double_taps();
  void fire_verified_taps();

  Vec2 tuio_to_pixels(const Vec2& pos) const;
  Vec2 tuio_to_meters(const Vec2& pos) const;

 protected:
  const double UNHANDLED_TP_REMOVE_TIME = 10.0;

  const double TAP_MAX_DURATION = 0.3;
  const double TAP_MAX_DISTANCE = 0.005;

  const double LONG_TAP_MIN_DURATION = 0.5;

  const double DOUBLE_TAP_MAX_DISTANCE = 0.02;
  const double DOUBLE_TAP_MAX_PAUSE = 0.2;

 private:
  std::unique_ptr<lo::ServerThread> m_liblo_st;

  Vec2 m_screen_resolution;
  Vec2 m_screen_size;

  std::map<uint32_t, std::shared_ptr<TouchPoint>> m_touch_points;
  std::deque<std::shared_ptr<TouchPoint>> m_unhandled_tps;

  std::deque<std::shared_ptr<Tap>> m_possible_taps;
};
