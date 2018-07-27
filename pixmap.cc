// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#include "output.h"
#include "pixmap.h"
#include "screen.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <fstream>

namespace {
  void toOStream(const std::vector<std::vector<uint8_t>>& pixels, std::ostream& os)
  {
    for (const auto& row : pixels) {
      for (const auto& c : row) {
        if (std::isprint(c))
          os << static_cast<unsigned char>(c);
        else
          os << ' ';
      }
      os << std::endl;
    }
  }
}

dcled::PixMap::PixMap(uint32_t width, uint32_t height)
  : pixels_(height, std::vector<uint8_t>(width))
{}

dcled::PixMap::PixMap(const Screen& screen)
  : PixMap(Screen::WIDTH, Screen::HEIGHT)
{
  for (auto x = decltype(Screen::WIDTH){0}; x < Screen::WIDTH; ++x) {
    for (auto y = decltype(Screen::HEIGHT){0}; y < Screen::HEIGHT; ++y) {
      at(x,y) = screen.get(x,y) ? '*' : ' ';
    }
  }
}

void dcled::PixMap::resize(uint32_t width, uint32_t height)
{
  const bool widthChange =  (width != this->width());
  const bool heightDecrease = (height < this->height());
  if (!heightDecrease && widthChange) {
    for (auto& row : pixels_) {
      row.resize(width);
    }
  }
  pixels_.resize( height, std::vector<uint8_t>(width) );
  if (heightDecrease && widthChange) {
    for (auto& row : pixels_) {
      row.resize(width);
    }
  }
}

dcled::PixMap dcled::PixMap::fromFile(const std::string& filename)
{
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    error() << "Could not open file '" << filename << "': " << std::strerror(errno);
    return PixMap();
  }
  std::list<std::string> rows;
  uint32_t width = 0, height = 0;
  size_t linecount = 0;
  auto pos = std::string::npos;
  for (std::string line; std::getline(infile, line); ++linecount)
  {
    // Skip empty lines and comment lines
    if (!line.size()) continue;
    if (line[0] == '#' || line[pos] == ';') continue;
    width = std::max(width, static_cast<uint32_t>(line.size()));
    rows.emplace_back(std::move(line));
    ++height;
  }

  PixMap pm(width, height);
  size_t y = 0;
  for (auto it = rows.cbegin(), end = rows.cend(); it != end; ++it, ++y) {
    for(size_t x = 0; x < it->size(); ++x ) {
      pm.at(static_cast<uint32_t>(x), static_cast<uint32_t>(y)) = (*it)[x];
    }
  }
  return pm;
}

bool dcled::PixMap::toFile(const std::string& filename) const
{
  std::ofstream outfile(filename);
  if (!outfile.is_open()) {
    error() << "Could not open file '" << filename << "': " << std::strerror(errno);
    return false;
  }
  toOStream(pixels_, outfile);
  return true;
}

void dcled::PixMap::toStdout() const
{
  toOStream(pixels_, std::cout);
}
