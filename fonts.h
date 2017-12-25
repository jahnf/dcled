#ifndef DCLED_FONT_H
#define DCLED_FONT_H

#include <string>

namespace dcled
{
  // Currently all fonts are compiled into the program, might change later.
  namespace font {
    struct Font {
      static constexpr uint8_t DEFAULT_CHAR[7] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};
      auto operator[](uint8_t c) const { return (c < 32) ? DEFAULT_CHAR : data[c-32]; }
      const uint8_t width;
      const uint8_t height = 7;
      const std::string name;
      const std::string desc;
      const std::string author;
      const uint8_t data[224][7];
    };

    // The default font
    extern const Font def;

    // A smaller font
    extern const Font small;
  }
}

#endif // DCLED_FONT_H
