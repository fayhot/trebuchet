#include <recognizers/tap_recognizer.hpp>

TapRecognizer::TapRecognizer(const Vec2& screen_resolution,
                             const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

std::set<TouchPointPtr> TapRecognizer::recognize(
    const std::set<TouchPointPtr>& touch_points) {
  std::set<TouchPointPtr> used_tps;
  std::vector<TouchPointPtr> tps;
  std::set_difference(touch_points.begin(), touch_points.end(),
                      m_used_touch_points.begin(), m_used_touch_points.end(),
                      std::inserter(tps, tps.begin()));
  for (auto& tp : tps) {
    auto dist =
        distance(tuio_to_meters(tp->pos()), tuio_to_meters(tp->start_pos()));
    if (tp->finished() && tp->duration() >= TAP_MIN_DURATION &&
        tp->duration() < TAP_MAX_DURATION && dist < TAP_MAX_DISTANCE) {
      used_tps.insert(tp);
      m_taps.emplace(std::make_shared<Tap>(tp));
      m_used_touch_points.insert(tp);
    }
  }

  return used_tps;
}

std::set<GestureEventPair> TapRecognizer::update() {
  // create tap events
  auto double_taps = verified_double_taps();
  auto taps = verified_taps();

  // return all events as one set
  taps.merge(double_taps);
  return taps;
}

bool TapRecognizer::invalidate_touch_point(const TouchPointPtr& touch_point) {
  bool was_tp_used = false;

  for (auto it = m_taps.begin(); it != m_taps.end();) {
    auto tap = *it;
    auto tps = tap->touch_points();
    if (tps.find(touch_point) != tps.end()) {
      was_tp_used = true;
      it = m_taps.erase(it);
    } else {
      ++it;
    }
  }

  return was_tp_used;
}

std::set<GestureEventPair> TapRecognizer::verified_double_taps() {
  std::set<GestureEventPair> events;

  for (auto&& taps : iter::combinations(m_taps, 2)) {
    // check if all of these taps are still in the taps set
    if (!std::all_of(taps.begin(), taps.end(), [&](auto& tap) {
          return m_taps.find(tap) != m_taps.end();
        })) {
      continue;
    }

    // compute the properties of the tap combination
    auto pause =
        taps[0]->touch_point()->end_time() - taps[1]->touch_point()->end_time();
    auto dist =
        distance(taps[0]->touch_point()->pos(), taps[1]->touch_point()->pos());

    // check if the two taps form a double tap
    if (pause < DOUBLE_TAP_MAX_PAUSE && dist < DOUBLE_TAP_MAX_DISTANCE) {
      auto double_tap = std::make_shared<DoubleTap>(taps[0]->touch_point(),
                                                    taps[1]->touch_point());
      events.emplace(std::make_pair(double_tap, GestureEvent::TRIGGER));
      m_used_touch_points.erase(taps[0]->touch_point());
      m_used_touch_points.erase(taps[1]->touch_point());
      m_taps.erase(taps[0]);
      m_taps.erase(taps[1]);
    }
  }

  return events;
}

std::set<GestureEventPair> TapRecognizer::verified_taps() {
  std::set<GestureEventPair> events;

  auto now = Clock::now();
  for (auto it = m_taps.begin(); it != m_taps.end();) {
    auto tap = *it;
    auto time_since_finished = now - tap->end_time();
    if (time_since_finished > DOUBLE_TAP_MAX_PAUSE) {
      events.emplace(std::make_pair(tap, GestureEvent::TRIGGER));
      m_used_touch_points.erase(tap->touch_point());
      it = m_taps.erase(it);
    } else {
      ++it;
    }
  }
  return events;
}
