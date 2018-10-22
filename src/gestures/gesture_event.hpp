#pragma once

enum class GestureEvent { POSSIBLE, START, END, TRIGGER };

std::ostream& operator<<(std::ostream& stream, GestureEvent event) {
  switch (event) {
    case GestureEvent::START:
      stream << "start";
      break;
    case GestureEvent::TRIGGER:
      stream << "trigger";
      break;
    case GestureEvent::END:
      stream << "end";
      break;
    default:
      stream << "unknown";
  }
  return stream;
}
