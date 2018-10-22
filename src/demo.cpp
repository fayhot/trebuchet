#include <memory>
#include <thread>

#include <gesture_recognizer.hpp>

int main(int argc, char const* argv[]) {
  auto recognizer = std::make_unique<GestureRecognizer>();
  recognizer->start();

  while (true) {
    auto gesture_events = recognizer->update();
    for (auto event : gesture_events) {
      switch (event.state) {
        case GestureState::START:
          std::cout << "start ";
          break;
        case GestureState::TRIGGER:
          std::cout << "trigger ";
          break;
        case GestureState::END:
          std::cout << "end ";
          break;
      }
      event.gesture->print(std::cout);
      std::cout << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
