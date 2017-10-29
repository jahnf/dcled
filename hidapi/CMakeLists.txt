cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

find_package(LibUSB REQUIRED)

add_library(hidapi-libusb STATIC libusb/hid.c)

#TODO fix findLibUSB to always have correct include dirs...
get_filename_component(LIBUSB1_INCLUDE_DIR "${LibUSB_HEADER_FILE}" DIRECTORY)
target_include_directories(hidapi-libusb BEFORE
  PRIVATE
    ${LIBUSB1_INCLUDE_DIR}
    hidapi/hidapi
  PUBLIC
    hidapi
)
target_link_libraries(hidapi-libusb INTERFACE ${LibUSB_LIBRARIES} pthread)

find_package(udev REQUIRED)
message(STATUS "## ${UDEV_LIBRARIES}")

add_library(hidapi-hidraw STATIC linux/hid.c)
target_include_directories(hidapi-hidraw BEFORE
  PRIVATE
    ${UDEV_INCLUDE_DIR}
    hidapi/hidapi
  PUBLIC
    hidapi
)
target_link_libraries(hidapi-hidraw PUBLIC ${UDEV_LIBRARIES})
