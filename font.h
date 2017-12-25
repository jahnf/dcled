#ifndef DCLED_FONT_H
#define DCLED_FONT_H

#include <string>
#include <array>


namespace dcled
{
  class Font {
  public:
    static constexpr uint8_t FONT_HEIGHT = 7; // currently all fonts have the same height

    Font(Font&& font) = default;

    auto operator[](uint8_t character) const {
      return data_[character];
    }

    auto width() const { return width_; }
    auto height() const { return height_; }

    static Font createDefault();

    // interfaces
    //static Font fromFile(const std::string& fileName);
  private:
    Font() = default;
    std::string name_;
    std::string desc_;
    std::string author_;
    uint8_t width_;
    const uint8_t height_ = FONT_HEIGHT;
    uint8_t data_[256][FONT_HEIGHT];
  };


}

#endif // DCLED_FONT_H
