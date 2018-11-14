#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/stl.h>

#include <application.hpp>
#include <gestures/gesture.hpp>
#include <gestures/gesture_event.hpp>
#include <gestures/tap.hpp>
#include <gestures/double_tap.hpp>
#include <gestures/long_tap.hpp>
#include <gestures/fling.hpp>
#include <gestures/pinch.hpp>
#include <gestures/swipe.hpp>
#include <vec2.hpp>

namespace py = pybind11;

std::tuple<double, double> vec_to_tuple(const Vec2& vec) {
  return std::make_pair(vec.x, vec.y);
}

PYBIND11_MODULE(trebuchet, m) {
  m.doc() = "A touch gesture recognition library based on TUIO1.1";

  py::class_<TouchPoint, std::shared_ptr<TouchPoint>>(m, "TouchPoint")
      .def_property_readonly("id", &TouchPoint::id)
      .def_property_readonly(
          "start_pos",
          [](const TouchPoint& tp) { return vec_to_tuple(tp.start_pos()); })
      .def_property_readonly(
          "pos", [](const TouchPoint& tp) { return vec_to_tuple(tp.pos()); })
      .def_property_readonly(
          "velocity",
          [](const TouchPoint& tp) { return vec_to_tuple(tp.velocity()); })
      .def_property_readonly("acceleration", &TouchPoint::acceleration)
      .def_property_readonly(
          "direction",
          [](const TouchPoint& tp) { return vec_to_tuple(tp.direction()); })
      .def_property_readonly("start_time", &TouchPoint::start_time)
      .def_property_readonly("update_time", &TouchPoint::update_time)
      .def_property_readonly("end_time", &TouchPoint::end_time)
      .def_property_readonly("finished", &TouchPoint::finished)
      .def_property_readonly("duration", &TouchPoint::duration);

  py::enum_<GestureEvent>(m, "GestureEvent")
      .value("POSSIBLE", GestureEvent::POSSIBLE)
      .value("START", GestureEvent::START)
      .value("END", GestureEvent::END)
      .value("TRIGGER", GestureEvent::TRIGGER);

  py::class_<Gesture, std::shared_ptr<Gesture>>(m, "Gesture")
      .def(py::init<>())
      .def("__repr__", &Gesture::as_string)
      .def_property_readonly("touch_points", &Gesture::touch_points)
      .def_property_readonly("start_pos",
                             [](const Gesture& gesture) {
                               return vec_to_tuple(gesture.start_pos());
                             })
      .def_property_readonly(
          "pos",
          [](const Gesture& gesture) { return vec_to_tuple(gesture.pos()); })
      .def_property_readonly("velocity",
                             [](const Gesture& gesture) {
                               return vec_to_tuple(gesture.velocity());
                             })
      .def_property_readonly("start_time", &Gesture::start_time)
      .def_property_readonly("end_time", &Gesture::end_time)
      .def_property_readonly("duration", &Gesture::duration)
      .def_property_readonly("finished", &Gesture::finished);

  py::class_<Tap, std::shared_ptr<Tap>, Gesture>(m, "Tap")
      .def_property_readonly("touch_point", &Tap::touch_point);

  py::class_<DoubleTap, std::shared_ptr<DoubleTap>, Gesture>(m, "DoubleTap");

  py::class_<LongTap, std::shared_ptr<LongTap>, Gesture>(m, "LongTap");

  py::class_<Pinch, std::shared_ptr<Pinch>, Gesture>(m, "Pinch")
      .def_property_readonly(
          "first_center",
          [](const Pinch& pinch) { return vec_to_tuple(pinch.first_center()); })
      .def_property_readonly("second_center",
                             [](const Pinch& pinch) {
                               return vec_to_tuple(pinch.second_center());
                             })
      .def_property_readonly(
          "center",
          [](const Pinch& pinch) { return vec_to_tuple(pinch.center()); })
      .def_property_readonly("distance", &Pinch::distance)
      .def_property_readonly("horizontal", &Pinch::horizontal)
      .def_property_readonly("vertical", &Pinch::vertical)
      .def_property_readonly("num_fingers", &Pinch::num_fingers)
      .def_property_readonly(
          "start_center",
          [](const Pinch& pinch) { return vec_to_tuple(pinch.start_center()); })
      .def_property_readonly("start_distance", &Pinch::start_distance);

  py::class_<Fling, std::shared_ptr<Fling>, Gesture>(m, "Fling")
      .def_property_readonly(
          "direction",
          [](const Fling& fling) { return vec_to_tuple(fling.direction()); })
      .def_property_readonly("num_fingers", &Fling::num_fingers)
      .def_property_readonly("speed", &Fling::speed);

  py::class_<Swipe, std::shared_ptr<Swipe>, Gesture>(m, "Swipe")
      .def_property_readonly("positions", [](const Swipe& swipe) {
        std::vector<std::tuple<double, double>> pos_tuples;
        auto positions = swipe.positions();
        pos_tuples.reserve(positions.size());
        std::transform(positions.begin(), positions.end(),
                       std::back_inserter(pos_tuples),
                       [](auto& pos) { return vec_to_tuple(pos); });
        return pos_tuples;
      });

  py::class_<Application>(m, "Application")
      .def(py::init<>())
      .def(py::init([](std::tuple<uint32_t, uint32_t> screen_resolution,
                       std::tuple<double, double> screen_size,
                       uint32_t port = 3333) {
        return std::make_unique<Application>(
            Vec2(std::get<0>(screen_resolution),
                 std::get<1>(screen_resolution)),
            Vec2(std::get<0>(screen_size), std::get<1>(screen_size)), port);
      }))
      .def_property(
          "screen_resolution",
          [](Application& app) {
            return vec_to_tuple(app.get_screen_resolution());
          },
          [](Application& app, const std::tuple<double, double>& resolution) {
            app.set_screen_resolution(
                Vec2(std::get<0>(resolution), std::get<1>(resolution)));
          })
      .def_property(
          "screen_size",
          [](Application& app) { return vec_to_tuple(app.get_screen_size()); },
          [](Application& app, const std::tuple<double, double>& size) {
            app.set_screen_size(Vec2(std::get<0>(size), std::get<1>(size)));
          })
      .def("update", &Application::update)
      .def("start", &Application::start);
}
