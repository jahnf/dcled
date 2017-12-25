#include "animations.h"

#include "font.h"
#include "screen.h"

namespace dcled {
  void printchar(Screen& s, Font &f, uint8_t x_pos, uint8_t c) {
    for ( uint8_t cx, x, y = 0; y < f.height() && y < Screen::HEIGHT; ++y) {
      for (cx = 0, x = x_pos; x < Screen::WIDTH && cx <= f.width(); ++cx, ++x) {
        if (f[c][y] & (1 << cx))
          s.set(x, y, true);
      }
    }
  }

  FontAnimation1::FontAnimation1()
    : f_(Font::createDefault()) {}

  uint32_t FontAnimation1::step(Screen& screen)
  {
    if( counter > 21 )
      return 0;

    if( counter++ == 0 ) {
      screen.setAll(false);
      printchar(screen, f_, 0, 'H');
      printchar(screen, f_, 5, 'e');
      printchar(screen, f_, 10, 'L');
      printchar(screen, f_, 15, 'l');
      printchar(screen, f_, 20, 'o');
      return 2000;
    }

    screen.shift(Screen::Direction::Left);
    return 100;
  }

  void FontAnimation1::reset()
  {
    counter =  0;
  }
}

uint32_t dcled::ExitAnimation::step(Screen& screen)
{
  if( counter <= 0 )
    return 0;

  screen.setAll( (--counter)%2 );
  return 150;
}

uint32_t dcled::ExitAnimation2::step(Screen& screen)
{
  if( counter > 21 )
    return 0;

  if( counter++ == 0 ) {
    screen.setAll(true);
    return 100;
  }

  screen.shift(Screen::Direction::Left);
  return 100;
}
