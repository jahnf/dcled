#ifndef DCLED_ANIMATIONS_H
#define DCLED_ANIMATIONS_H

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

  // TODO : blinking animation of last screen with given interval and count
  class BlinkingAnimation : public Animation
  {
  public:
    BlinkingAnimation(uint32_t interval_ms, uint32_t count = 1);
    virtual ~BlinkingAnimation() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override { counter_ = 10; }
  private:
    Screen s_;
    int counter_ = 10;
  };

  class TestAnimation1 : public Animation
  {
  public:
    virtual ~TestAnimation1() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override { counter_= 0;}
  private:
    int counter_ = 0;
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

  //namespace font{ struct Font; }
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

#endif // DCLED_ANIMATIONS_H
