# CMake file for dcled-hidapi
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dcled-hidapi CXX)

option(BUILD_WITH_HIDAPI_LIBUSB "Build dcled-hidapi with hidapi-libusb" ON)
option(BUILD_WITH_HIDAPI_RAW "Build dcled-hidapi with hidapi-raw" OFF)

try_compile(HAVE_CODECVT
  "${CMAKE_BINARY_DIR}/compile-tests"
  "${CMAKE_SOURCE_DIR}/cmake/compiler-tests/codecvt.cc"
  OUTPUT_VARIABLE CODECVT_COMPILE_RUN
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)

if(NOT HAVE_CODECVT)
  message(FATAL_ERROR "<codecvt> NOT FOUND. If you use gcc, a switch to a newer version (5 or higher) will help.")
endif()

try_compile(CAN_INITIALIZE_FONT_STRUCT
  "${CMAKE_BINARY_DIR}/compile-tests"
  "${CMAKE_SOURCE_DIR}/fonts.cc;${CMAKE_SOURCE_DIR}/cmake/compiler-tests/default-main.cc"
  OUTPUT_VARIABLE FONT_STRUCT_COMPILE_RUN
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)

if(NOT CAN_INITIALIZE_FONT_STRUCT)
  message(FATAL_ERROR "Cannot initialize font structs with initializer list. "
                      "If you use gcc, a switch to a newer version (5 or higher) will help.")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-D__STRICT_ANSI__)
endif()

add_subdirectory(hidapi)

add_library(dcledlib STATIC EXCLUDE_FROM_ALL
  animations.cc animations.h
  device.cc device.h
  fonts.cc fonts.h
  pixmap.cc pixmap.h
  screen.cc screen.h
  moodycamel/concurrentqueue.h
)
target_include_directories(dcledlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE hidapi)

if(BUILD_WITH_HIDAPI_LIBUSB)
  add_executable(dcled dcled-main.cc dcled-cli.cc dcled-cli.h args/args.hxx)
  target_link_libraries(dcled dcledlib hidapi-libusb pthread)
  target_include_directories(dcled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(TARGET dcled PROPERTY CXX_STANDARD 14)
endif()

if(BUILD_WITH_HIDAPI_RAW)
  add_executable(dcled-hidapi-raw dcled-main.cc dcled-cli.cc dcled-cli.h args/args.hxx)
  target_link_libraries(dcled-raw dcledlib hidapi-hidraw pthread)
  target_include_directories(dcled-raw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(TARGET dcled-raw PROPERTY CXX_STANDARD 14)
endif()

add_custom_target(non-sources SOURCES README.md 40-dcled.rules)
