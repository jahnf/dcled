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

find_package (Threads)

if(BUILD_DCLED_HIDAPI_LIBUSB OR BUILD_DCLED_HIDAPI_RAW)
  add_library(dcledlib STATIC EXCLUDE_FROM_ALL ${dcledlib_SOURCES})
  target_include_directories(dcledlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE hidapi)
  target_link_libraries (dcledlib INTERFACE ${CMAKE_THREAD_LIBS_INIT})
  # if(BUILD_SHARED_LIBS)
  #   add_library(dcled-shared SHARED ${dcledlib_SOURCES})
  #   target_include_directories(dcled-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE hidapi)
  #   add_version_info_custom_prefix(dcled-shared dcled  "${CMAKE_CURRENT_SOURCE_DIR}")
  #   set_target_properties(dcled-shared PROPERTIES OUTPUT_NAME "dcled")
  # endif()
endif()

if(BUILD_DCLED_HIDAPI_LIBUSB OR BUILD_DCLED_HIDAPI_RAW)
  add_library(dcledcli STATIC EXCLUDE_FROM_ALL dcled-main.cc dcled-cli.cc dcled-cli.h args/args.hxx)
  target_include_directories(dcledcli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  add_version_info_custom_prefix(dcledcli dcled  "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

if(BUILD_DCLED_HIDAPI_LIBUSB)
  add_executable(dcledc output.h)
  target_link_libraries(dcledc dcledcli dcledlib hidapi-libusb)
endif()

# For Ubuntu/Debian systems `libudev-dev` pkg is required to build HIDAPI_RAW version.
if(BUILD_DCLED_HIDAPI_RAW)
  add_executable(dcledc-raw output.h)
  target_link_libraries(dcledc-raw dcledcli dcledlib hidapi-hidraw)
endif()

if(BUILD_DCLED_WITHOUT_HIDAPI)
  add_library(dcledlib-nohidapi STATIC EXCLUDE_FROM_ALL ${dcledlib_SOURCES})
  target_include_directories(dcledlib-nohidapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_definitions(dcledlib-nohidapi PUBLIC WITHOUT_HIDAPI)
  target_link_libraries(dcledlib-nohidapi INTERFACE ${CMAKE_THREAD_LIBS_INIT})
  add_library(dcledcli-nohidapi STATIC EXCLUDE_FROM_ALL dcled-main.cc dcled-cli.cc dcled-cli.h args/args.hxx)
  target_include_directories(dcledcli-nohidapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_definitions(dcledcli-nohidapi PUBLIC WITHOUT_HIDAPI)
  add_version_info_custom_prefix(dcledcli-nohidapi dcled  "${CMAKE_CURRENT_SOURCE_DIR}")
  add_executable(dcledc-nohidapi output.h)
  target_link_libraries(dcledc-nohidapi dcledcli-nohidapi dcledlib-nohidapi)
endif()

add_custom_target(non-sources SOURCES README.md LICENSE 40-dcled.rules .travis.yml)
