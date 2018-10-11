#include <gesture_recognizer.hpp>

GestureRecognizer::GestureRecognizer(uint32_t port) {
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

void GestureRecognizer::refresh(TUIO::TuioTime frameTime) {}
