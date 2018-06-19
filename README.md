# dcled (hidapi)

Userland driver for Dream Cheeky (Dream Link?) USB LED Message Board.

Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>

While this is orginally a fork of https://github.com/kost/dcled by Jeff Jahr,
it ended up with competely different code.

While the original idea was just to make `dcled` work with
[HID API](http://www.signal11.us/oss/hidapi/) and therefore run on my current
Ubuntu (and OpenSuse) systems, it became kind of a toy project of mine for C++ and CMake.

## Third party

* `args` (https://github.com/Taywee/args) by Taylor C. Richberger <taywee@gmx.com> and
Pavel Belikov is used for argumen t parsing.

* `moodycamel::ConcurrentQueue` (https://github.com/cameron314/concurrentqueue)
  by Cameron Desrochers is used.

* And of course `HID API` (https://github.com/signal11/hidapi).

## Building

**Requirements**

* C++14 compiler (if gcc, then version 5 or higher)
* CMake 3.8 or later
* libusb when building the default version with hidapi-libusb

**How to build:**

      > git clone https://github.com/jahnf/dcled-hidapi
      > cd dcled-hidapi
      > mkdir build && cd build
      > cmake ..
      > make

**Build options**

Besides the default CMake build options, the following is available:
* `BUILD_DCLED_HIDAPI_LIBUSB` - Build a dcled version with hidapi-libusb, default = ON
* `BUILD_DCLED_HIDAPI_RAW` - Build a dcled version with hidapi-raw, default = OFF
* `BUILD_DCLED_WITHOUT_HIDAPI` - Build a dcled version without any hidapi/usb bindings, default = OFF

Either tell CMake the options on the command line or use the CMake convenience tools `ccmake` or `cmake-gui`.

## Features

While not all features from the original `dcled` have (yet) been implemented,
there are new features available.

* Basically dcled (hidapi) take a list of animations,
  puts them in the devices queue and it is processed.

* This can be via the command line or via files (with animation lists inside) or a mix of it.

* Even if you don't have a USB Messageboard connected you can show the animations
  on your (color-enabled) terminal. (`-s` and `-v` command line options)

## Running

If you want to connect to a Dream Cheeky USB Messageboard,
you need to give yourself write acces to this device.

* For this copy the file `40-dcled.rules` to  `/etc/udev/rules.d/40-dcled.rules`
* Run `sudo udevadm control --reload-rules`
  and `sudo udevadm trigger` to load rules without rebooting.

*(Tested on Ubuntu 16.04 and 18.04)*

## Other

* See Jeff Jahr's original page: https://www.last-outpost.com/~malakai/dcled/

## Roadmap/Ideas

* More animation types
* Support piping
* Support pixmaps in animations
* dcled-server: running as a daemon, offering a REST API?
* Maybe: Support Microsoft Windows builds
