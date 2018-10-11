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

  void detect_taps();

  Vec2 tuio_to_pixels(const Vec2& pos) const;
  Vec2 tuio_to_meters(const Vec2& pos) const;

 protected:
  const double UNHANDLED_TP_REMOVE_TIME = 10.0;

  const double TAP_MAX_DURATION = 0.3;

 private:
  std::unique_ptr<TUIO::TuioClient> m_tuio_client;

  Vec2 m_screen_resolution;
  Vec2 m_screen_size;

  std::map<uint32_t, std::shared_ptr<TouchPoint>> m_touch_points;
  std::deque<std::shared_ptr<TouchPoint>> m_unhandled_tps;
};
