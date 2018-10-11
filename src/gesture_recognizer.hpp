#pragma once

#include <memory>
#include <map>
#include <deque>

#include <TuioClient.h>
#include <TuioListener.h>

#include <touch_point.hpp>

class GestureRecognizer : public TUIO::TuioListener {
 public:
  GestureRecognizer(uint32_t port = 3333);
  GestureRecognizer(const GestureRecognizer&) = delete;
  ~GestureRecognizer() = default;

  void start();

 protected:
  virtual void addTuioObject(TUIO::TuioObject* tobj) override;
  virtual void updateTuioObject(TUIO::TuioObject* tobj) override;
  virtual void removeTuioObject(TUIO::TuioObject* tobj) override;
  virtual void addTuioCursor(TUIO::TuioCursor* tcur) override;
  virtual void updateTuioCursor(TUIO::TuioCursor* tcur) override;
  virtual void removeTuioCursor(TUIO::TuioCursor* tcur) override;
  virtual void addTuioBlob(TUIO::TuioBlob* tblb) override;
  virtual void updateTuioBlob(TUIO::TuioBlob* tblb) override;
  virtual void removeTuioBlob(TUIO::TuioBlob* tblb) override;
  virtual void refresh(TUIO::TuioTime frameTime) override;

 protected:
  const double UNHANDLED_TP_REMOVE_TIME = 10.0;

 private:
  std::unique_ptr<TUIO::TuioClient> m_tuio_client;

  std::map<uint32_t, std::shared_ptr<TouchPoint>> m_touch_points;
  std::deque<std::shared_ptr<TouchPoint>> m_unhandled_tps;
};
