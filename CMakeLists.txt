# CMake file for dcled-hidapi
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dcled-hidapi CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
include(GitVersion)

option(BUILD_DCLED_HIDAPI_LIBUSB "Build dcled-hidapi with hidapi-libusb" ON)
option(BUILD_DCLED_HIDAPI_RAW "Build dcled-hidapi with hidapi-raw" OFF)
option(BUILD_DCLED_WITHOUT_HIDAPI "Build dcled without hidapi/usb bindings" OFF)

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

if(BUILD_DCLED_HIDAPI_LIBUSB OR BUILD_DCLED_HIDAPI_RAW)
  add_subdirectory(hidapi)
endif()

set(dcledlib_SOURCES
  animations.cc animations.h
  device.cc device.h
  fonts.cc fonts.h
  pixmap.cc pixmap.h
  screen.cc screen.h
  moodycamel/concurrentqueue.h
)

if(BUILD_DCLED_HIDAPI_LIBUSB OR BUILD_DCLED_HIDAPI_RAW)
  add_library(dcledlib STATIC EXCLUDE_FROM_ALL ${dcledlib_SOURCES})
  target_include_directories(dcledlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE hidapi)
endif()

if(BUILD_DCLED_HIDAPI_LIBUSB)
  add_executable(dcled dcled-main.cc dcled-cli.cc dcled-cli.h args/args.hxx)
  target_link_libraries(dcled dcledlib hidapi-libusb pthread)
  target_include_directories(dcled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_version_info(dcled "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# For Ubuntu/Debian systems `libudev-dev` pkg is required to build HIDAPI_RAW version.
if(BUILD_DCLED_HIDAPI_RAW)
  add_executable(dcled-raw dcled-main.cc dcled-cli.cc dcled-cli.h args/args.hxx)
  target_link_libraries(dcled-raw dcledlib hidapi-hidraw pthread)
  target_include_directories(dcled-raw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_version_info_custom_prefix(dcled-raw dcled "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

if(BUILD_DCLED_WITHOUT_HIDAPI)
  add_library(dcledlib-nohidapi STATIC EXCLUDE_FROM_ALL ${dcledlib_SOURCES})
  target_include_directories(dcledlib-nohidapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_definitions( dcledlib-nohidapi PUBLIC WITHOUT_HIDAPI)
  add_executable(dcled-nohidapi dcled-main.cc dcled-cli.cc dcled-cli.h args/args.hxx)
  target_link_libraries(dcled-nohidapi dcledlib-nohidapi pthread)
  target_include_directories(dcled-nohidapi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_version_info_custom_prefix(dcled-nohidapi dcled "${CMAKE_CURRENT_SOURCE_DIR}")
endif()



add_custom_target(non-sources SOURCES README.md LICENSE 40-dcled.rules)
