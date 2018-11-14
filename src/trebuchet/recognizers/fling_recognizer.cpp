#include <recognizers/fling_recognizer.hpp>

FlingRecognizer::FlingRecognizer(const Vec2& screen_resolution,
                                 const Vec2& screen_size)
    : Recognizer(screen_resolution, screen_size) {}

std::set<GestureEventPair> FlingRecognizer::update(
    const std::set<TouchPointPtr>& touch_points) {
  detect_flings(touch_points);
  return verified_flings();
}

bool FlingRecognizer::invalidate_touch_point(const TouchPointPtr& touch_point) {
  bool was_tp_used = false;

  for (auto it = m_flings.begin(); it != m_flings.end();) {
    auto fling = *it;
    auto tps = fling->touch_points();
    if (tps.find(touch_point) != tps.end()) {
      was_tp_used = true;
      it = m_flings.erase(it);
    } else {
      ++it;
    }
  }

  return was_tp_used;
}

void FlingRecognizer::detect_flings(
    const std::set<TouchPointPtr>& touch_points) {
  for (auto& tp : touch_points) {
    // check if the conditions of a fling are met
    if (tuio_to_meters(tp->velocity()).length() >= MIN_VELOCITY) {
      bool part_of_other_fling = false;
      // check if this fling is part of another fling
      for (auto& fling : m_flings) {
        if (angle(fling->velocity(), tp->velocity()) < MAX_ANGLE_DIFF &&
            distance(tuio_to_meters(fling->pos()), tuio_to_meters(tp->pos())) <
                MAX_DISTANCE) {
          if (fling->add_touch_point(tp)) {
            part_of_other_fling = true;
            break;
          }
        }
      }

      // create a new fling gesture if this touch point was not part of
      // another fling
      if (!part_of_other_fling) {
        m_flings.emplace(std::make_shared<Fling>(tp));
      }
    }
  }
}

std::set<GestureEventPair> FlingRecognizer::verified_flings() {
  std::set<GestureEventPair> events;

  for (auto it = m_flings.begin(); it != m_flings.end();) {
    auto fling = *it;
    if (fling->duration() > MULTI_FINGER_MAX_TIME_BETWEEN) {
      events.emplace(std::make_pair(fling, GestureEvent::START));
      it = m_flings.erase(it);
    } else {
      ++it;
    }
  }

  return events;
}
