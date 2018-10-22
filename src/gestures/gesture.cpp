#include <gestures/gesture.hpp>

Gesture::Gesture(const std::deque<std::shared_ptr<TouchPoint>>& tps)
    : m_touch_points(tps) {}

Gesture::Gesture(Gesture&& other) noexcept {
  std::swap(m_touch_points, other.m_touch_points);
}

Gesture& Gesture::operator=(Gesture&& other) noexcept {
  if (this != &other) {
    std::swap(m_touch_points, other.m_touch_points);
  }
  return *this;
}

const std::deque<std::shared_ptr<TouchPoint>> Gesture::touch_points() const {
  return m_touch_points;
}

bool Gesture::finished() const {
  for (auto tp : m_touch_points) {
    if (!tp->finished()) {
      return false;
    }
  }
  return true;
}

double Gesture::time_finished() const {
  auto now = std::chrono::high_resolution_clock::now();
  std::deque<double> times;
  for (auto tp : m_touch_points) {
    if (tp->finished()) {
      times.emplace_back(std::chrono::duration_cast<std::chrono::milliseconds>(
                             now - tp->end_time())
                             .count() /
                         1000.0);
    }
  }
  return *std::min_element(times.begin(), times.end());
}

std::ostream& Gesture::print_touch_points(std::ostream& stream) const {
  for (auto it = m_touch_points.begin(); it != m_touch_points.end(); ++it) {
    stream << std::to_string((*it)->id());
    if (it != m_touch_points.end() - 1) {
      stream << " ";
    }
  }
  return stream;
}

std::ostream& Gesture::print(std::ostream& stream) const {
  stream << "Gesture <";
  print_touch_points(stream);
  return stream << ">";
}
