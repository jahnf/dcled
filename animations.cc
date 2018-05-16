#include "animations.h"

#include "fonts.h"
#include "screen.h"

#include <iostream>
namespace {
  struct print
  {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cout << a; }
    ~print() { std::cout << std::endl; }
  };
  struct error
  {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cerr << a; }
    ~error() { std::cerr << std::endl; }
  };
}

namespace dcled {

  ShowScreenAnimation::ShowScreenAnimation(const Screen& s, uint32_t time_ms )
    : s_(s), time_ms_(time_ms) {
    print() << "                    Create: ShowScreenAnimation()" << this;
  }
  ShowScreenAnimation::~ShowScreenAnimation( )
  {
    print() << "                   Destroy: ShowScreenAnimation()" << this;
  }


  void printchar(Screen& s, const font::Font &f, uint8_t c, uint8_t x_pos, uint8_t y_pos = 0) {
    for (uint8_t cx, cy = 0, x; cy < f.height && y_pos < Screen::HEIGHT; ++cy, ++y_pos) {
      for (cx = 0, x = x_pos; x < Screen::WIDTH && cx <= f.width; ++cx, ++x) {
        if (f[c][cy] & (1 << cx))
          s.set(x, y_pos, true);
      }
    }
  }

  TextAnimation::TextAnimation(const std::string& text, const font::Font& font)
    : font_(font), text_(text) {}

  TextAnimation::TextAnimation(const std::string& text, ScrollSpeed speed, const font::Font& font)
    : font_(font), text_(text), scrollspeed_(static_cast<uint32_t>(speed)) {
    print() << "Create TextAnimation " << this;
  }

  TextAnimation::TextAnimation(const std::string& text, uint32_t speed, const font::Font& font)
    : font_(font), text_(text), scrollspeed_(speed) {}

  TextAnimation::~TextAnimation()
  {
    print() << "Destroy TextAnimation " << this;
  }

  uint32_t TextAnimation::step(Screen& screen)
  {
    if( cur_char_pix_ > 21 )
      return 0;

    if( cur_char_ < text_.size() ) {
      screen.shift(Screen::Direction::Left);
      printchar(screen, font_, static_cast<uint8_t>(text_[cur_char_]),
                               Screen::WIDTH-(++cur_char_pix_));

      if (cur_char_pix_ >= font_.width) {
        ++cur_char_;
        cur_char_pix_ = 0;
      }
    }
    else {
      screen.shift(Screen::Direction::Left);
      ++cur_char_pix_;
    }
    return static_cast<uint32_t>(scrollspeed_);
  }

  void TextAnimation::reset()
  {
    cur_char_ = cur_char_pix_ = 0;
  }

  uint32_t ShowScreenAnimation::step(Screen& screen)
  {
    if( !done_ ) {
      screen = s_;
      done_ = true;
      return time_ms_;
    }
    return 0;
  }
}

uint32_t dcled::BlinkingAnimation::step(Screen& screen)
{
  if( counter_ <= 0 )
    return 0;

  screen.setAll( (--counter_)%2 );
  return 150;
}

uint32_t dcled::TestAnimation1::step(Screen& screen)
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
