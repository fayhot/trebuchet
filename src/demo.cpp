#include <memory>
#include <thread>

#include <gesture_recognizer.hpp>

int main(int argc, char const* argv[]) {
  auto recognizer = std::make_unique<GestureRecognizer>();
  recognizer->start();

  while (true) {
    recognizer->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
