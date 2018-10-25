cmake_minimum_required(VERSION 3.2)

project(trebuchet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# build the liblo library
add_subdirectory(external/liblo/cmake)

# build the trebuchet library
set(SOURCE_DIR "src/trebuchet")
include_directories(external)
include_directories(${SOURCE_DIR})
set(TREBUCHET_SOURCES
  "${SOURCE_DIR}/vec2.cpp"
  "${SOURCE_DIR}/gesture_recognizer.cpp"
  "${SOURCE_DIR}/touch_point.cpp"

  "${SOURCE_DIR}/gestures/gesture.cpp"
  "${SOURCE_DIR}/gestures/gesture_event.cpp"
  "${SOURCE_DIR}/gestures/tap.cpp"
  "${SOURCE_DIR}/gestures/long_tap.cpp"
  "${SOURCE_DIR}/gestures/double_tap.cpp"
  "${SOURCE_DIR}/gestures/pinch.cpp"
  "${SOURCE_DIR}/gestures/fling.cpp"
)
add_subdirectory(external/pybind11)
pybind11_add_module(trebuchet
  ${TREBUCHET_SOURCES}
  "${SOURCE_DIR}/bindings.cpp"
)
target_link_libraries(trebuchet PRIVATE lo_shared)
