cmake_minimum_required(VERSION 3.2)

project(trebuchet)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# build the TUIO library
set(TUIO_DIR "external/tuio")
include_directories("${TUIO_DIR}/TUIO")
include_directories("${TUIO_DIR}/oscpack")
add_library(tuio SHARED
  "${TUIO_DIR}/TUIO/TuioTime.cpp"
  "${TUIO_DIR}/TUIO/TuioPoint.cpp"
  "${TUIO_DIR}/TUIO/TuioContainer.cpp"
  "${TUIO_DIR}/TUIO/TuioObject.cpp"
  "${TUIO_DIR}/TUIO/TuioCursor.cpp"
  "${TUIO_DIR}/TUIO/TuioBlob.cpp"
  "${TUIO_DIR}/TUIO/TuioDispatcher.cpp"
  "${TUIO_DIR}/TUIO/TuioManager.cpp"
  "${TUIO_DIR}/TUIO/OneEuroFilter.cpp"

  "${TUIO_DIR}/TUIO/TuioClient.cpp"
  "${TUIO_DIR}/TUIO/OscReceiver.cpp"
  "${TUIO_DIR}/TUIO/UdpReceiver.cpp"
  "${TUIO_DIR}/TUIO/TcpReceiver.cpp"

  "${TUIO_DIR}/oscpack/osc/OscTypes.cpp"
  "${TUIO_DIR}/oscpack/osc/OscOutboundPacketStream.cpp"
  "${TUIO_DIR}/oscpack/osc/OscReceivedElements.cpp"
  "${TUIO_DIR}/oscpack/osc/OscPrintReceivedElements.cpp"
  "${TUIO_DIR}/oscpack/ip/posix/NetworkingUtils.cpp"
  "${TUIO_DIR}/oscpack/ip/posix/UdpSocket.cpp"
)
target_link_libraries(tuio pthread)

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
target_link_libraries(trebuchet tuio)

# build the demo application
add_executable(demo "${SOURCE_DIR}/demo.cpp")
target_link_libraries(demo trebuchet)
