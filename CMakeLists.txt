# CMake file for dcled-hidapi
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)

project(dcled-hidapi)

option(BUILD_WITH_HIDAPI_LIBUSB "Build dcled-hidapi with hidapi-libusb" ON)
option(BUILD_WITH_HIDAPI_RAW "Build dcled-hidapi with hidapi-raw" OFF)

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
  add_executable(dcled dcled-main.cc args/args.hxx)
  target_link_libraries(dcled dcledlib hidapi-libusb pthread)
  target_include_directories(dcled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(TARGET dcled PROPERTY CXX_STANDARD 14)
endif()

if(BUILD_WITH_HIDAPI_RAW)
  add_executable(dcled-hidapi-raw dcled-main.cc args/args.hxx)
  target_link_libraries(dcled-raw dcledlib hidapi-hidraw pthread)
  target_include_directories(dcled-raw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(TARGET dcled-raw PROPERTY CXX_STANDARD 14)
endif()
