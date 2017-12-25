#include "animations.h"

#include "fonts.h"
#include "screen.h"

namespace dcled {
  void printchar(Screen& s, const font::Font &f, uint8_t c, uint8_t x_pos, uint8_t y_pos = 0) {
    for (uint8_t cx, cy = 0, x; cy < f.height && y_pos < Screen::HEIGHT; ++cy, ++y_pos) {
      for (cx = 0, x = x_pos; x < Screen::WIDTH && cx <= f.width; ++cx, ++x) {
        if (f[c][cy] & (1 << cx))
          s.set(x, y_pos, true);
      }
    }
  }

  FontAnimation1::FontAnimation1()
    : font_(font::small) {}

  uint32_t FontAnimation1::step(Screen& screen)
  {
    if( counter_ > 21 )
      return 0;

    if( counter_++ == 0 ) {
      screen.setAll(false);
      printchar(screen, font_, 'H', 0, 1);
      printchar(screen, font_, 'e', 4, 1);
      printchar(screen, font_, 'L', 8, 1);
      printchar(screen, font_, 'l', 12, 1);
      printchar(screen, font_, 'o', 16, 1);
      return 2000;
    }

    screen.shift(Screen::Direction::Left);
    return 100;
  }

  void FontAnimation1::reset()
  {
    counter_ =  0;
  }
}

uint32_t dcled::ExitAnimation::step(Screen& screen)
{
  if( counter_ <= 0 )
    return 0;

  screen.setAll( (--counter_)%2 );
  return 150;
}

uint32_t dcled::ExitAnimation2::step(Screen& screen)
{
  if( counter_ > 21 )
    return 0;

  if( counter_++ == 0 ) {
    screen.setAll(true);
    return 100;
  }

  screen.shift(Screen::Direction::Left);
  return 100;
}
