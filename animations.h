// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#pragma once

#include "fonts.h"
#include "screen.h"

namespace dcled {

  enum class ScrollSpeed : uint16_t  {
    Fast = 50 , Medium = 100, Slow = 150
  };

  class Animation
  {
  public:
    virtual ~Animation() = default;
    /// Updates the screen with the next animation step if the end is not reached and returns
    /// the time in ms that the updated screen should be shown. Returns 0 if the end of the
    /// animation was reached.
    virtual uint32_t step(Screen&) { return 0; }
    /// Resets the animation to the beginning.
    virtual void reset() = 0;
  };

  class NestedAnimation : public Animation
  {
  public:
    NestedAnimation(std::unique_ptr<Animation> a) : animation_(std::move(a)) {}
    virtual ~NestedAnimation() = default;
    virtual uint32_t step(Screen& s);
    virtual void reset() override;
  private:
    Screen s_;
    std::unique_ptr<Animation> animation_;
    bool first_step_ = true;
  };

  class InvertAnimation : public NestedAnimation
  {
  public:
    InvertAnimation(std::unique_ptr<Animation> a) : NestedAnimation(std::move(a)) {}
    virtual ~InvertAnimation() = default;
    virtual uint32_t step(Screen& s) override
    {
      const auto step_ms = NestedAnimation::step(s);
      if (step_ms) s.invert();
      return step_ms;
    }
  };

  class FlipAnimation : public NestedAnimation
  {
  public:
    FlipAnimation(std::unique_ptr<Animation> a, Screen::Flip direction = Screen::Flip::Horizontal)
    : NestedAnimation(std::move(a)), flip_direction_(direction) {}
    virtual ~FlipAnimation() = default;
    virtual uint32_t step(Screen& s) override
    {
      const auto step_ms = NestedAnimation::step(s);
      if (step_ms) s.flip(flip_direction_);
      return step_ms;
    }
  private:
    const Screen::Flip flip_direction_ = Screen::Flip::Horizontal;
  };

  /// Shows a digital clock in either 24 or 12 hour format, for a given time or forever.
  class ClockAnimation : public Animation
  {
  public:
    enum class Mode { H24 = 24, H12 = 12 };
    ClockAnimation( uint32_t display_time_s = 0, bool blinking_colon = true, Mode mode = Mode::H24 );
    virtual ~ClockAnimation() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override;
  private:
    uint32_t display_time_s_ = 0; // 0: indefinitely
    uint64_t time_left_ms_ = 0;
    bool blinking_colon_ = true;
    bool colon_show_state_ = true;
    Mode mode_ = Mode::H24;
  };

  /// Shows a screen for the given time.
  class ShowScreenAnimation : public Animation
  {
  public:
    ShowScreenAnimation(const Screen& s, uint32_t time_ms = 1000 );
    virtual ~ShowScreenAnimation() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override { done_ = false; }
  private:
    Screen s_;
    uint32_t time_ms_ = 1000;
    bool done_ = false;
  };

  /// Scrolling test animation with a given text, scrollspeed and font.
  class TextAnimation : public Animation
  {
  public:
    TextAnimation(const std::string& text, const font::Font& font);
    TextAnimation(const std::string& text, uint32_t speed, const font::Font& font = font::Default);
    TextAnimation(const std::string& text, ScrollSpeed speed = ScrollSpeed::Medium,
                  const font::Font& font = font::Default);
    virtual ~TextAnimation() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override;
  private:
    const font::Font& font_;
    const std::string text_;
    uint32_t scrollspeed_ = static_cast<uint32_t>(ScrollSpeed::Medium);
    uint32_t cur_char_ = 0;
    uint8_t cur_char_pix_ = 0;
  };
}
