#ifndef DCLED_ANIMATIONS_H
#define DCLED_ANIMATIONS_H

//#include <cstdint>

#include "font.h"

namespace dcled {

  class Screen;
  class Animation
  {
  public:
    virtual ~Animation() = default;
    /// Updates the screen with the next animation step if the end is not reached and return
    /// the time in ms that the updated screen should be shown. Returns 0 if the end of the
    /// animation was reached.
    virtual uint32_t step(Screen&) { return 0; }
    /// Resets the animation to the beginning.
    virtual void reset() = 0;
  };

  class ExitAnimation : public Animation
  {
  public:
    virtual ~ExitAnimation() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override { counter_ = 10; }
  private:
    int counter_ = 10;
  };

  class ExitAnimation2 : public Animation
  {
  public:
    virtual ~ExitAnimation2() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override { counter_= 0;}
  private:
    int counter_ = 0;
  };

  class Font;
  class FontAnimation1 : public Animation
  {
  public:
    FontAnimation1(const std::string& text);
    virtual ~FontAnimation1() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override;
  private:
    Font f_;
    std::string text_;
    int counter_ = 0;
  };
}

#endif // DCLED_ANIMATIONS_H
