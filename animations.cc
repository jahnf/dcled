// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#include "animations.h"

#include "fonts.h"
#include "screen.h"

#include <ctime>
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
    : s_(s), time_ms_(time_ms) {}

  void printchar(Screen& s, const font::Font &f, uint8_t c,
                 uint8_t x_pos, uint8_t y_pos = 0, bool transparent = true, bool invert = false) {
    for (uint8_t cx, cy = 0, x; cy < f.height && y_pos < Screen::HEIGHT; ++cy, ++y_pos) {
      for (cx = 0, x = x_pos; x < Screen::WIDTH && cx <= f.width; ++cx, ++x) {
        if (bool on = (f[c][cy] & (1 << cx)))
          s.set(x, y_pos, true);
        else if (!transparent)
          s.set(x, y_pos, false);
      }
    }
  }

  ClockAnimation::ClockAnimation(uint32_t display_time_s, bool blinking_colon, Mode mode)
    : display_time_s_(display_time_s), time_left_ms_(display_time_s*1000),
      blinking_colon_(blinking_colon), mode_(mode) {}

  void ClockAnimation::reset() {
    time_left_ms_ = display_time_s_ * 1000;
  }

  uint32_t ClockAnimation::step(Screen & screen)
  {
    const auto t = std::time(nullptr);
    char clock_string[5] = "0000";
    std::strftime(clock_string, sizeof(clock_string), "%H%M", std::localtime(&t));
    printchar(screen, font::Default, clock_string[0], 0, 0, false);
    printchar(screen, font::Default, clock_string[1], 5, 0, false);
    screen.set(10, 2, colon_show_state_).set(10, 4, colon_show_state_);
    printchar(screen, font::Default, clock_string[2], 12, 0, false);
    printchar(screen, font::Default, clock_string[3], 17, 0, false);
    colon_show_state_ = blinking_colon_ ? !colon_show_state_ : true;
    if (display_time_s_) {
      if (time_left_ms_ > 500) {
        time_left_ms_ -= 500;
      }
      else {
        auto return_ms = time_left_ms_;
        time_left_ms_ = 0;
        return return_ms;
      }
    }
    return 500;
  }

  TextAnimation::TextAnimation(const std::string& text, const font::Font& font)
    : font_(font), text_(text) {}

  TextAnimation::TextAnimation(const std::string& text, ScrollSpeed speed, const font::Font& font)
    : font_(font), text_(text), scrollspeed_(static_cast<uint32_t>(speed)) {}

  TextAnimation::TextAnimation(const std::string& text, uint32_t speed, const font::Font& font)
    : font_(font), text_(text), scrollspeed_(speed) {}

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


