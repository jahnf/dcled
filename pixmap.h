#ifndef PIXMAP_H
#define PIXMAP_H

#include <vector>
#include <cstdint>

namespace dcled
{
  class Screen;
  class PixMap {
  public:
    PixMap(uint32_t width, uint32_t height);
    PixMap(const Screen& screen);
    PixMap(PixMap&& other) = default;
    PixMap(const PixMap& other) = default;
    PixMap& operator=(PixMap&& other) = default;
    PixMap& operator=(const PixMap& other) = default;

    //TODO:
    static PixMap fromFile();
    //-----------

    const uint8_t& at(uint32_t x, uint32_t y) const {
      return pixels_.at(x + y * width_);
    }

    uint8_t& at(uint32_t x, uint32_t y) {
      return pixels_.at(x + y * width_);
    }

    uint32_t width() const { return width_; }
    uint32_t height() const { return height_; }

  private:
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::vector<uint8_t> pixels_;
  };

} // end namespace dcled
#endif // PIXMAP_H
