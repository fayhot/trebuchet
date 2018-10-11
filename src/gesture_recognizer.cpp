#include <gesture_recognizer.hpp>

GestureRecognizer::GestureRecognizer(uint32_t port)
    : m_screen_resolution(1920.0, 1080.0), m_screen_size(0.597, 0.336) {
  m_tuio_client = std::make_unique<TUIO::TuioClient>(port);
}

void GestureRecognizer::start() {
  m_tuio_client->addTuioListener(this);
  m_tuio_client->connect();
}

void GestureRecognizer::addTuioObject(TUIO::TuioObject* tobj) {}

void GestureRecognizer::updateTuioObject(TUIO::TuioObject* tobj) {}

void GestureRecognizer::removeTuioObject(TUIO::TuioObject* tobj) {}

void GestureRecognizer::addTuioCursor(TUIO::TuioCursor* tcur) {
  auto id = tcur->getSessionID();
  auto tp = std::make_shared<TouchPoint>(tcur);
  m_touch_points.emplace(std::make_pair(id, tp));
  m_unhandled_tps.push_back(tp);
}

void GestureRecognizer::updateTuioCursor(TUIO::TuioCursor* tcur) {
  auto id = tcur->getSessionID();
  auto tp = m_touch_points.at(id);
  tp->update(tcur);
}

void GestureRecognizer::removeTuioCursor(TUIO::TuioCursor* tcur) {
  auto id = tcur->getSessionID();
  auto tp = m_touch_points.at(id);
  tp->end();
}

void GestureRecognizer::addTuioBlob(TUIO::TuioBlob* tblb) {}

void GestureRecognizer::updateTuioBlob(TUIO::TuioBlob* tblb) {}

void GestureRecognizer::removeTuioBlob(TUIO::TuioBlob* tblb) {}

void GestureRecognizer::refresh(TUIO::TuioTime frameTime) {
  detect_taps();

  // remove unhandled touch points that are not active for some time
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    if (tp->finished_since() > UNHANDLED_TP_REMOVE_TIME) {
      m_touch_points.erase(tp->id());
      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

void GestureRecognizer::detect_taps() {
  for (auto it = m_unhandled_tps.begin(); it != m_unhandled_tps.end();) {
    auto tp = *it;
    auto dist =
        (tuio_to_meters(tp->pos() - tuio_to_meters(tp->start_pos()))).length();
    if (tp->finished() && tp->duration() < TAP_MAX_DURATION &&
        dist < TAP_MAX_DISTANCE) {
      std::cout << "TAP" << std::endl;
      it = m_unhandled_tps.erase(it);
    } else {
      ++it;
    }
  }
}

Vec2 GestureRecognizer::tuio_to_pixels(const Vec2& pos) const {
  return pos * m_screen_resolution;
}

Vec2 GestureRecognizer::tuio_to_meters(const Vec2& pos) const {
  return pos * m_screen_size;
}
