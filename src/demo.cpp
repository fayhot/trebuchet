#include <memory>

#include <gesture_recognizer.hpp>

int main(int argc, char const* argv[]) {
  auto recognizer = std::make_unique<GestureRecognizer>();
  recognizer->start();

  while (true) {
  }

  return 0;
}
