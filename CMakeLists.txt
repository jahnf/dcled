# CMake file for dcled-hidapi
cmake_minimum_required(VERSION 3.1)

project(dcled-hidapi)
add_subdirectory(hidapi)

set(SOURCES
  main.cpp
  dcledmessageboard.cpp
  args/args.hxx
)


add_executable(dcled ${SOURCES})
target_link_libraries(dcled hidapi-libusb pthread)
target_include_directories(dcled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET dcled PROPERTY CXX_STANDARD 14)

add_executable(dcled-raw ${SOURCES})
target_link_libraries(dcled-raw hidapi-hidraw pthread)
target_include_directories(dcled-raw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET dcled-raw PROPERTY CXX_STANDARD 14)
