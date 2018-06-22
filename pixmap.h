// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace dcled
{
  class Screen;
  class PixMap {
  public:
    PixMap(uint32_t width = 0, uint32_t height = 0);
    PixMap(const Screen& screen);
    PixMap(PixMap&& other) = default;
    PixMap(const PixMap& other) = default;
    PixMap& operator=(PixMap&& other) = default;
    PixMap& operator=(const PixMap& other) = default;

    static PixMap fromFile(const std::string& filename);
    bool toFile(const std::string& filename) const;
    void toStdout() const;

    const uint8_t& at(uint32_t x, uint32_t y) const {
      return pixels_.at(y).at(x);
    }

    uint8_t& at(uint32_t x, uint32_t y) {
      return pixels_.at(y).at(x);
    }

    uint32_t width() const { return pixels_.size() ? pixels_.front().size() : 0; }
    uint32_t height() const { return pixels_.size(); }

    void resize(uint32_t width, uint32_t height);

  private:
    std::vector<std::vector<uint8_t>> pixels_;
  };

} // end namespace dcled
