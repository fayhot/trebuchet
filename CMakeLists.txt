cmake_minimum_required(VERSION 3.2)

project(trebuchet)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# build the liblo library
add_subdirectory(external/liblo/cmake)

# build the trebuchet library
set(SOURCE_DIR "src")
include_directories(${SOURCE_DIR})
set(TREBUCHET_SOURCES
  "${SOURCE_DIR}/vec2.cpp"
  "${SOURCE_DIR}/gesture_recognizer.cpp"
  "${SOURCE_DIR}/touch_point.cpp"
  "${SOURCE_DIR}/gesture.cpp"
  "${SOURCE_DIR}/gestures/tap.cpp"
)
add_library(trebuchet SHARED ${TREBUCHET_SOURCES})
target_link_libraries(trebuchet lo_shared)

# build the demo application
add_executable(demo "${SOURCE_DIR}/demo.cpp")
target_link_libraries(demo trebuchet)
