// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#include "pixmap.h"
#include "screen.h"

dcled::PixMap::PixMap(uint32_t width, uint32_t height)
  : width_(width), height_(height), pixels_(width * height)
{}

dcled::PixMap::PixMap(const Screen& screen)
  : PixMap(Screen::WIDTH, Screen::HEIGHT)
{
  for (auto x = decltype(Screen::WIDTH){0}; x < Screen::WIDTH; ++x) {
    for (auto y = decltype(Screen::HEIGHT){0}; y < Screen::HEIGHT; ++y) {
      at(x,y) = screen.get(x,y);
    }
  }
}
