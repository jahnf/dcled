// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#ifndef DCLED_SCREEN_H
#define DCLED_SCREEN_H

#include <vector>
#include <list>
#include <memory>

namespace dcled
{
  class PixMap;
  class Screen {
  public:
    static constexpr uint8_t WIDTH = 21;
    static constexpr uint8_t HEIGHT = 7;

    enum class Brightness : uint8_t { Low = 0, Mid = 1, High = 2 };
    enum class Direction : uint8_t { Left, Right, Up, Down };

    constexpr Screen(): msgs_{LedMsg(0), LedMsg(2), LedMsg(4), LedMsg(6)} {}
    Screen(const PixMap& pixmap, uint32_t x_offset = 0, uint32_t y_offset = 0);
    Screen(const Screen& other) = default;
    Screen(Screen&& other) = default;
    virtual ~Screen() = default;

    Screen& operator=(const Screen& other) = default;
    Screen& operator=(Screen&& other) = default;

    Screen& operator&(const Screen& other);
    Screen& operator|(const Screen& other);
    Screen& operator^(const Screen& other);

    Screen& invert();
    Screen& shift(Direction dir);
    Screen& setBrightness(Brightness brightness);
    Screen& setAll(bool on);
    Screen& setRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool on);
    Screen& set(uint8_t x, uint8_t y, bool on);
    Screen& set(const Screen& other);

    /// Get the LED state at \a x, \a y.
    bool get(uint8_t x, uint8_t y) const;

    void print(bool ttyColored = false) const;

  protected:
    struct LedMsg
    {
      constexpr LedMsg(uint8_t row) : row(row) {}
      const LedMsg& operator&=(const LedMsg& other);
      const LedMsg& operator|=(const LedMsg& other);
      const LedMsg& operator^=(const LedMsg& other);
      void invert();
      void setAll(bool on);
      uint8_t report_id = 0;
      Brightness brightness = Brightness::High;
      uint8_t row;
      uint8_t data[2][3] = {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}}; // all LEDs off
    };
    LedMsg msgs_[4];
  };
} // end namespace dcled
#endif // DCLED_SCREEN_H
