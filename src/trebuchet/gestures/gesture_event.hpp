#pragma once

#include <iostream>

enum class GestureEvent { POSSIBLE, START, END, TRIGGER };

std::ostream& operator<<(std::ostream& stream, GestureEvent event);
