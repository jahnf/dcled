#include "screen.h"
#include "pixmap.h"

#include <iostream>

namespace {
  union BitShift {
    uint8_t c[4];
    uint32_t i;
  };
}

void dcled::Screen::print(bool ttyColored) const
{
  for (uint8_t y = 0; y < Screen::HEIGHT; ++y) {
    std::cout << "|";
    for (uint8_t x = 0; x < Screen::WIDTH; ++x) {
      if (ttyColored)
        std::cout << (get(x,y) ? "\x1b[1;37;41m*\033[0m" : "\x1b[90;40m.\033[0m" );
      else
        std::cout << (get(x,y) ? "*" : "." );
    }
    std::cout << '|' << std::endl;
  }
}

dcled::Screen::Screen(const PixMap& pixmap, uint32_t x, uint32_t y)
  : Screen()
{
  const auto x_max = std::min(x + Screen::WIDTH, x);
  const auto y_max = std::min(y + Screen::HEIGHT, y);
  for (auto screen_x = decltype(Screen::WIDTH){0}; x < x_max; ++x, ++screen_x) {
    for (auto screen_y = decltype(Screen::HEIGHT){0}; y < y_max; ++y, ++screen_y) {
      set(screen_x, screen_y, pixmap.at(x,y));
    }
  }
}

dcled::Screen& dcled::Screen::setBrightness(Brightness brightness)
{
  msgs_[0].brightness = brightness;
  msgs_[1].brightness = brightness;
  msgs_[2].brightness = brightness;
  msgs_[3].brightness = brightness;
  return *this;
}

dcled::Screen& dcled::Screen::setAll(bool on)
{
  msgs_[0].setAll(on);
  msgs_[1].setAll(on);
  msgs_[2].setAll(on);
  msgs_[3].setAll(on);
  return *this;
}

bool dcled::Screen::get(uint8_t x, uint8_t y) const
{
  if (y < HEIGHT && x < WIDTH)
    return !(msgs_[y/2].data[y%2][2-x/8] & (1 << x%8));

  return false;
}

dcled::Screen& dcled::Screen::set(uint8_t x, uint8_t y, bool on)
{
  if (!(y < HEIGHT && x < WIDTH))
    return *this;

  if (on)
    msgs_[y/2].data[y%2][2-x/8] &= ~(1 << x%8);
  else
    msgs_[y/2].data[y%2][2-x/8] |= (1 << x%8);
  return *this;
}

dcled::Screen& dcled::Screen::setRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool on)
{
  if (!(y < HEIGHT && x < WIDTH))
    return *this;

  for (w += x, h += y; x < w && y < h; ++x, ++y)
    set(x, y, on);

  return *this;
}

dcled::Screen& dcled::Screen::operator|(const Screen& other)
{
  msgs_[0] |= other.msgs_[0];
  msgs_[1] |= other.msgs_[1];
  msgs_[2] |= other.msgs_[2];
  msgs_[3] |= other.msgs_[3];
  return *this;
}

dcled::Screen& dcled::Screen::operator&(const Screen& other)
{
  msgs_[0] &= other.msgs_[0];
  msgs_[1] &= other.msgs_[1];
  msgs_[2] &= other.msgs_[2];
  msgs_[3] &= other.msgs_[3];
  return *this;
}

dcled::Screen& dcled::Screen::operator^(const Screen& other)
{
  msgs_[0] ^= other.msgs_[0];
  msgs_[1] ^= other.msgs_[1];
  msgs_[2] ^= other.msgs_[2];
  msgs_[3] ^= other.msgs_[3];
  return *this;
}

dcled::Screen& dcled::Screen::shift(Direction dir)
{
  switch(dir) {
  case Direction::Up:
    for (int i=0; i<4; ++i) {
      msgs_[i].data[0][0] = msgs_[i].data[1][0];
      msgs_[i].data[0][1] = msgs_[i].data[1][1];
      msgs_[i].data[0][2] = msgs_[i].data[1][2];
      if (i<3) {
        msgs_[i].data[1][0] = msgs_[i+1].data[0][0];
        msgs_[i].data[1][1] = msgs_[i+1].data[0][1];
        msgs_[i].data[1][2] = msgs_[i+1].data[0][2];
      }
    }
    msgs_[3].data[0][0] = msgs_[3].data[0][1] = msgs_[3].data[0][2] = 0xFF;
    break;
  case Direction::Down:
    for (int i=3; i>=0; --i) {
      msgs_[i].data[1][0] = msgs_[i].data[0][0];
      msgs_[i].data[1][1] = msgs_[i].data[0][1];
      msgs_[i].data[1][2] = msgs_[i].data[0][2];
      if (i>0) {
        msgs_[i].data[0][0] = msgs_[i-1].data[1][0];
        msgs_[i].data[0][1] = msgs_[i-1].data[1][1];
        msgs_[i].data[0][2] = msgs_[i-1].data[1][2];
      }
    }
    msgs_[0].data[0][0] = msgs_[0].data[0][1] = msgs_[0].data[0][2] = 0xFF;
    break;
  case Direction::Left:
    for (int i=0; i<4; ++i) {
      for (int j=0; j<2; ++j) {
        msgs_[i].data[j][0] |= (5<<5);
        BitShift t{ { msgs_[i].data[j][2], msgs_[i].data[j][1], msgs_[i].data[j][0], 0xFF} };
        t.i >>= 1;
        msgs_[i].data[j][2] = t.c[0];
        msgs_[i].data[j][1] = t.c[1];
        msgs_[i].data[j][0] = t.c[2];
      }
    }
    break;
  case Direction::Right:
    for (int i=0; i<4; ++i) {
      for (int j=0; j<2; ++j) {
        BitShift t{ {0xFF, msgs_[i].data[j][2], msgs_[i].data[j][1], msgs_[i].data[j][0]} };
        t.i <<= 1;
        msgs_[i].data[j][2] = t.c[1];
        msgs_[i].data[j][1] = t.c[2];
        msgs_[i].data[j][0] = t.c[3];
      }
    }
    break;
  }
  return *this;
}

dcled::Screen& dcled::Screen::invert()
{
  msgs_[0].invert();
  msgs_[1].invert();
  msgs_[2].invert();
  msgs_[3].invert();
  return *this;
}

void dcled::Screen::LedMsg::setAll(bool on)
{
  data[0][0] = data[0][1] = data[0][2] = on ? 0 : 0xFF;
  data[1][0] = data[1][1] = data[1][2] = on ? 0 : 0xFF;
}

const dcled::Screen::LedMsg& dcled::Screen::LedMsg::operator&=(const LedMsg& other)
{
  data[0][0] |= other.data[0][0];
  data[0][1] |= other.data[0][1];
  data[0][2] |= other.data[0][2];
  data[1][0] |= other.data[1][0];
  data[1][1] |= other.data[1][1];
  data[1][2] |= other.data[1][2];
  return *this;
}

const dcled::Screen::LedMsg& dcled::Screen::LedMsg::operator|=(const LedMsg& other)
{
  data[0][0] &= other.data[0][0];
  data[0][1] &= other.data[0][1];
  data[0][2] &= other.data[0][2];
  data[1][0] &= other.data[1][0];
  data[1][1] &= other.data[1][1];
  data[1][2] &= other.data[1][2];
  return *this;
}

const dcled::Screen::LedMsg& dcled::Screen::LedMsg::operator^=(const LedMsg& other)
{
  data[0][0] ^= ~other.data[0][0];
  data[0][1] ^= ~other.data[0][1];
  data[0][2] ^= ~other.data[0][2];
  data[1][0] ^= ~other.data[1][0];
  data[1][1] ^= ~other.data[1][1];
  data[1][2] ^= ~other.data[1][2];
  return *this;
}

void dcled::Screen::LedMsg::invert()
{
  data[0][0] = ~data[0][0];
  data[0][1] = ~data[0][1];
  data[0][2] = ~data[0][2];
  data[1][0] = ~data[1][0];
  data[1][1] = ~data[1][1];
  data[1][2] = ~data[1][2];
}
