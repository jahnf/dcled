#include "fonts.h"

namespace dcled {
  namespace font {
    constexpr uint8_t Font::DEFAULT_CHAR[];

    const Font def{5, 7, "default", "The default font, taken from X11's 5x7", "Jeff Jahr",
    {
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x04, 0x04, 0x04, 0x04, 0x00, 0x04, 0x00 },
      { 0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x00 },
      { 0x00, 0x0E, 0x05, 0x0E, 0x14, 0x0E, 0x00 }, { 0x01, 0x09, 0x04, 0x02, 0x09, 0x08, 0x00 },
      { 0x00, 0x02, 0x05, 0x02, 0x05, 0x0A, 0x00 }, { 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00 },
      { 0x04, 0x02, 0x02, 0x02, 0x02, 0x04, 0x00 }, { 0x02, 0x04, 0x04, 0x04, 0x04, 0x02, 0x00 },
      { 0x00, 0x0A, 0x04, 0x0E, 0x04, 0x0A, 0x00 }, { 0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x02 }, { 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00 }, { 0x00, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00 },
      { 0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x04, 0x00 }, { 0x04, 0x06, 0x04, 0x04, 0x04, 0x0E, 0x00 },
      { 0x06, 0x09, 0x08, 0x04, 0x02, 0x0F, 0x00 }, { 0x0F, 0x08, 0x06, 0x08, 0x09, 0x06, 0x00 },
      { 0x04, 0x06, 0x05, 0x0F, 0x04, 0x04, 0x00 }, { 0x0F, 0x01, 0x07, 0x08, 0x09, 0x06, 0x00 },
      { 0x06, 0x01, 0x07, 0x09, 0x09, 0x06, 0x00 }, { 0x0F, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00 },
      { 0x06, 0x09, 0x06, 0x09, 0x09, 0x06, 0x00 }, { 0x06, 0x09, 0x09, 0x0E, 0x08, 0x06, 0x00 },
      { 0x00, 0x06, 0x06, 0x00, 0x06, 0x06, 0x00 }, { 0x00, 0x06, 0x06, 0x00, 0x06, 0x02, 0x01 },
      { 0x00, 0x08, 0x04, 0x02, 0x04, 0x08, 0x00 }, { 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x00 },
      { 0x00, 0x02, 0x04, 0x08, 0x04, 0x02, 0x00 }, { 0x04, 0x0A, 0x08, 0x04, 0x00, 0x04, 0x00 },
      { 0x06, 0x09, 0x0D, 0x0D, 0x01, 0x06, 0x00 }, { 0x06, 0x09, 0x09, 0x0F, 0x09, 0x09, 0x00 },
      { 0x07, 0x09, 0x07, 0x09, 0x09, 0x07, 0x00 }, { 0x06, 0x09, 0x01, 0x01, 0x09, 0x06, 0x00 },
      { 0x07, 0x09, 0x09, 0x09, 0x09, 0x07, 0x00 }, { 0x0F, 0x01, 0x07, 0x01, 0x01, 0x0F, 0x00 },
      { 0x0F, 0x01, 0x07, 0x01, 0x01, 0x01, 0x00 }, { 0x06, 0x09, 0x01, 0x0D, 0x09, 0x0E, 0x00 },
      { 0x09, 0x09, 0x0F, 0x09, 0x09, 0x09, 0x00 }, { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 },
      { 0x08, 0x08, 0x08, 0x08, 0x09, 0x06, 0x00 }, { 0x09, 0x05, 0x03, 0x03, 0x05, 0x09, 0x00 },
      { 0x01, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x00 }, { 0x09, 0x0F, 0x0F, 0x09, 0x09, 0x09, 0x00 },
      { 0x09, 0x0B, 0x0B, 0x0D, 0x0D, 0x09, 0x00 }, { 0x06, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 },
      { 0x07, 0x09, 0x09, 0x07, 0x01, 0x01, 0x00 }, { 0x06, 0x09, 0x09, 0x09, 0x0B, 0x06, 0x08 },
      { 0x07, 0x09, 0x09, 0x07, 0x05, 0x09, 0x00 }, { 0x06, 0x09, 0x02, 0x04, 0x09, 0x06, 0x00 },
      { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00 }, { 0x09, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 },
      { 0x09, 0x09, 0x09, 0x09, 0x06, 0x06, 0x00 }, { 0x09, 0x09, 0x09, 0x0F, 0x0F, 0x09, 0x00 },
      { 0x09, 0x09, 0x06, 0x06, 0x09, 0x09, 0x00 }, { 0x0A, 0x0A, 0x0A, 0x04, 0x04, 0x04, 0x00 },
      { 0x0F, 0x08, 0x04, 0x02, 0x01, 0x0F, 0x00 }, { 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x00 },
      { 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00 }, { 0x0E, 0x08, 0x08, 0x08, 0x08, 0x0E, 0x00 },
      { 0x04, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00 },
      { 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x0E, 0x09, 0x0D, 0x0A, 0x00 },
      { 0x01, 0x01, 0x07, 0x09, 0x09, 0x07, 0x00 }, { 0x00, 0x00, 0x06, 0x01, 0x01, 0x06, 0x00 },
      { 0x08, 0x08, 0x0E, 0x09, 0x09, 0x0E, 0x00 }, { 0x00, 0x00, 0x06, 0x0D, 0x03, 0x06, 0x00 },
      { 0x04, 0x0A, 0x02, 0x07, 0x02, 0x02, 0x00 },
      // This is the g converted from the 5x7 font: { 0x00, 0x00, 0x0E, 0x09, 0x06, 0x01, 0x0E },
      // This one is from Andy Scheller:
      { 0x00, 0x00, 0x0E, 0x09, 0x0E, 0x08, 0x06 },
      // end
      { 0x01, 0x01, 0x07, 0x09, 0x09, 0x09, 0x00 }, { 0x04, 0x00, 0x06, 0x04, 0x04, 0x0E, 0x00 },
      { 0x08, 0x00, 0x08, 0x08, 0x08, 0x0A, 0x04 }, { 0x01, 0x01, 0x05, 0x03, 0x05, 0x09, 0x00 },
      { 0x06, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 }, { 0x00, 0x00, 0x05, 0x0F, 0x09, 0x09, 0x00 },
      { 0x00, 0x00, 0x07, 0x09, 0x09, 0x09, 0x00 }, { 0x00, 0x00, 0x06, 0x09, 0x09, 0x06, 0x00 },
      { 0x00, 0x00, 0x07, 0x09, 0x09, 0x07, 0x01 }, { 0x00, 0x00, 0x0E, 0x09, 0x09, 0x0E, 0x08 },
      { 0x00, 0x00, 0x07, 0x09, 0x01, 0x01, 0x00 }, { 0x00, 0x00, 0x0E, 0x03, 0x0C, 0x07, 0x00 },
      { 0x02, 0x02, 0x07, 0x02, 0x02, 0x0C, 0x00 }, { 0x00, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x00 },
      { 0x00, 0x00, 0x0A, 0x0A, 0x0A, 0x04, 0x00 }, { 0x00, 0x00, 0x09, 0x09, 0x0F, 0x0F, 0x00 },
      { 0x00, 0x00, 0x09, 0x06, 0x06, 0x09, 0x00 }, { 0x00, 0x00, 0x09, 0x09, 0x0A, 0x04, 0x02 },
      { 0x00, 0x00, 0x0F, 0x04, 0x02, 0x0F, 0x00 }, { 0x08, 0x04, 0x06, 0x04, 0x04, 0x08, 0x00 },
      { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00 }, { 0x02, 0x04, 0x0C, 0x04, 0x04, 0x02, 0x00 },
      { 0x0A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }, { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x04, 0x00, 0x04, 0x04, 0x04, 0x04, 0x00 },
      { 0x00, 0x04, 0x0E, 0x05, 0x05, 0x0E, 0x04 }, { 0x00, 0x0C, 0x02, 0x07, 0x02, 0x0D, 0x00 },
      { 0x00, 0x11, 0x0E, 0x0A, 0x0E, 0x11, 0x00 }, { 0x0A, 0x0A, 0x04, 0x0E, 0x04, 0x04, 0x00 },
      { 0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00 }, { 0x0C, 0x02, 0x06, 0x0A, 0x0C, 0x08, 0x06 },
      { 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x0E, 0x11, 0x15, 0x13, 0x15, 0x11, 0x0E },
      { 0x06, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x12, 0x09, 0x12, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x0F, 0x08, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00 },
      { 0x0E, 0x11, 0x17, 0x13, 0x13, 0x11, 0x0E }, { 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x04, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00 }, { 0x04, 0x04, 0x1F, 0x04, 0x04, 0x1F, 0x00 },
      { 0x06, 0x04, 0x02, 0x06, 0x00, 0x00, 0x00 }, { 0x06, 0x06, 0x04, 0x06, 0x00, 0x00, 0x00 },
      { 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x09, 0x09, 0x09, 0x07, 0x01 },
      { 0x0E, 0x0B, 0x0B, 0x0A, 0x0A, 0x0A, 0x00 }, { 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02 }, { 0x04, 0x06, 0x04, 0x0E, 0x00, 0x00, 0x00 },
      { 0x02, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x09, 0x12, 0x09, 0x00, 0x00 },
      { 0x01, 0x01, 0x01, 0x09, 0x0C, 0x0E, 0x08 }, { 0x01, 0x01, 0x01, 0x0D, 0x08, 0x04, 0x0C },
      { 0x03, 0x03, 0x02, 0x0B, 0x0C, 0x0E, 0x08 }, { 0x04, 0x00, 0x04, 0x02, 0x0A, 0x04, 0x00 },
      { 0x06, 0x09, 0x09, 0x0F, 0x09, 0x09, 0x00 }, { 0x06, 0x09, 0x09, 0x0F, 0x09, 0x09, 0x00 },
      { 0x06, 0x09, 0x09, 0x0F, 0x09, 0x09, 0x00 }, { 0x06, 0x09, 0x09, 0x0F, 0x09, 0x09, 0x00 },
      { 0x09, 0x06, 0x09, 0x0F, 0x09, 0x09, 0x00 }, { 0x06, 0x06, 0x09, 0x0F, 0x09, 0x09, 0x00 },
      { 0x0E, 0x05, 0x0D, 0x07, 0x05, 0x0D, 0x00 }, { 0x06, 0x09, 0x01, 0x01, 0x09, 0x06, 0x02 },
      { 0x0F, 0x01, 0x07, 0x01, 0x01, 0x0F, 0x00 }, { 0x0F, 0x01, 0x07, 0x01, 0x01, 0x0F, 0x00 },
      { 0x0F, 0x01, 0x07, 0x01, 0x01, 0x0F, 0x00 }, { 0x0F, 0x01, 0x07, 0x01, 0x01, 0x0F, 0x00 },
      { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 }, { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 },
      { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 }, { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 },
      { 0x07, 0x0A, 0x0B, 0x0A, 0x0A, 0x07, 0x00 }, { 0x0D, 0x09, 0x0B, 0x0D, 0x0D, 0x09, 0x00 },
      { 0x06, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 }, { 0x06, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 },
      { 0x06, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 }, { 0x06, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 },
      { 0x09, 0x06, 0x09, 0x09, 0x09, 0x06, 0x00 }, { 0x00, 0x00, 0x09, 0x06, 0x06, 0x09, 0x00 },
      { 0x0E, 0x0D, 0x0D, 0x0B, 0x0B, 0x07, 0x00 }, { 0x09, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 },
      { 0x09, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 }, { 0x09, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 },
      { 0x09, 0x00, 0x09, 0x09, 0x09, 0x06, 0x00 }, { 0x0A, 0x0A, 0x0A, 0x04, 0x04, 0x04, 0x00 },
      { 0x01, 0x07, 0x09, 0x07, 0x01, 0x01, 0x00 }, { 0x06, 0x09, 0x05, 0x09, 0x09, 0x05, 0x00 },
      { 0x02, 0x04, 0x0E, 0x09, 0x0D, 0x0A, 0x00 }, { 0x04, 0x02, 0x0E, 0x09, 0x0D, 0x0A, 0x00 },
      { 0x04, 0x0A, 0x0E, 0x09, 0x0D, 0x0A, 0x00 }, { 0x0A, 0x05, 0x0E, 0x09, 0x0D, 0x0A, 0x00 },
      { 0x0A, 0x00, 0x0E, 0x09, 0x0D, 0x0A, 0x00 }, { 0x06, 0x06, 0x0E, 0x09, 0x0D, 0x0A, 0x00 },
      { 0x00, 0x00, 0x0E, 0x0D, 0x05, 0x0E, 0x00 }, { 0x00, 0x00, 0x0C, 0x02, 0x02, 0x0C, 0x04 },
      { 0x02, 0x04, 0x06, 0x0D, 0x03, 0x06, 0x00 }, { 0x04, 0x02, 0x06, 0x0D, 0x03, 0x06, 0x00 },
      { 0x02, 0x05, 0x06, 0x0D, 0x03, 0x06, 0x00 }, { 0x05, 0x00, 0x06, 0x0D, 0x03, 0x06, 0x00 },
      { 0x02, 0x04, 0x06, 0x04, 0x04, 0x0E, 0x00 }, { 0x04, 0x02, 0x06, 0x04, 0x04, 0x0E, 0x00 },
      { 0x04, 0x0A, 0x06, 0x04, 0x04, 0x0E, 0x00 }, { 0x0A, 0x00, 0x06, 0x04, 0x04, 0x0E, 0x00 },
      { 0x02, 0x0C, 0x06, 0x09, 0x09, 0x06, 0x00 }, { 0x0A, 0x05, 0x07, 0x09, 0x09, 0x09, 0x00 },
      { 0x02, 0x04, 0x06, 0x09, 0x09, 0x06, 0x00 }, { 0x04, 0x02, 0x06, 0x09, 0x09, 0x06, 0x00 },
      { 0x06, 0x00, 0x06, 0x09, 0x09, 0x06, 0x00 }, { 0x0A, 0x05, 0x06, 0x09, 0x09, 0x06, 0x00 },
      { 0x0A, 0x00, 0x06, 0x09, 0x09, 0x06, 0x00 }, { 0x00, 0x06, 0x00, 0x0F, 0x00, 0x06, 0x00 },
      { 0x00, 0x00, 0x0E, 0x0D, 0x0B, 0x07, 0x00 }, { 0x02, 0x04, 0x09, 0x09, 0x09, 0x0E, 0x00 },
      { 0x04, 0x02, 0x09, 0x09, 0x09, 0x0E, 0x00 }, { 0x06, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x00 },
      { 0x0A, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x00 }, { 0x04, 0x02, 0x09, 0x09, 0x0A, 0x04, 0x02 },
      { 0x00, 0x01, 0x07, 0x09, 0x09, 0x07, 0x01 }, { 0x0A, 0x00, 0x09, 0x09, 0x0A, 0x04, 0x02 },
     }}; // Font def


    const Font small = {4, 5, "small", "Very small characters.", "Stefan Misch",
    {
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x02, 0x02, 0x02, 0x00, 0x02, 0x00, 0x00 },
      { 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x02, 0x07, 0x02, 0x07, 0x02, 0x00, 0x00 },
      { 0x06, 0x03, 0x02, 0x06, 0x03, 0x00, 0x00 }, { 0x05, 0x04, 0x02, 0x01, 0x05, 0x00, 0x00 },
      { 0x02, 0x05, 0x02, 0x05, 0x06, 0x00, 0x00 }, { 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x02, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00 }, { 0x02, 0x04, 0x04, 0x04, 0x02, 0x00, 0x00 },
      { 0x00, 0x05, 0x02, 0x05, 0x00, 0x00, 0x00 }, { 0x00, 0x02, 0x07, 0x02, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00 }, { 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00 }, { 0x04, 0x04, 0x02, 0x01, 0x01, 0x00, 0x00 },
      { 0x02, 0x05, 0x07, 0x05, 0x02, 0x00, 0x00 }, { 0x04, 0x06, 0x05, 0x04, 0x04, 0x00, 0x00 },
      { 0x02, 0x05, 0x02, 0x01, 0x07, 0x00, 0x00 }, { 0x03, 0x04, 0x02, 0x04, 0x03, 0x00, 0x00 },
      { 0x05, 0x05, 0x07, 0x04, 0x04, 0x00, 0x00 }, { 0x07, 0x01, 0x03, 0x04, 0x03, 0x00, 0x00 },
      { 0x02, 0x01, 0x03, 0x05, 0x02, 0x00, 0x00 }, { 0x07, 0x04, 0x02, 0x02, 0x02, 0x00, 0x00 },
      { 0x02, 0x05, 0x02, 0x05, 0x02, 0x00, 0x00 }, { 0x02, 0x05, 0x06, 0x04, 0x02, 0x00, 0x00 },
      { 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00 }, { 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00 },
      { 0x08, 0x04, 0x02, 0x04, 0x08, 0x00, 0x00 }, { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00 },
      { 0x01, 0x02, 0x04, 0x02, 0x01, 0x00, 0x00 }, { 0x02, 0x05, 0x04, 0x02, 0x00, 0x02, 0x00 },
      { 0x02, 0x05, 0x07, 0x01, 0x06, 0x00, 0x00 }, { 0x02, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00 },
      { 0x03, 0x05, 0x03, 0x05, 0x03, 0x00, 0x00 }, { 0x06, 0x01, 0x01, 0x01, 0x06, 0x00, 0x00 },
      { 0x03, 0x05, 0x05, 0x05, 0x03, 0x00, 0x00 }, { 0x07, 0x01, 0x03, 0x01, 0x07, 0x00, 0x00 },
      { 0x07, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00 }, { 0x02, 0x05, 0x01, 0x05, 0x06, 0x00, 0x00 },
      { 0x05, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00 }, { 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00 },
      { 0x04, 0x04, 0x04, 0x04, 0x03, 0x00, 0x00 }, { 0x05, 0x05, 0x03, 0x05, 0x05, 0x00, 0x00 },
      { 0x01, 0x01, 0x01, 0x01, 0x07, 0x00, 0x00 }, { 0x05, 0x07, 0x07, 0x05, 0x05, 0x00, 0x00 },
      { 0x05, 0x07, 0x07, 0x07, 0x05, 0x00, 0x00 }, { 0x02, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00 },
      { 0x03, 0x05, 0x03, 0x01, 0x01, 0x00, 0x00 }, { 0x02, 0x05, 0x05, 0x05, 0x06, 0x00, 0x00 },
      { 0x03, 0x05, 0x03, 0x05, 0x05, 0x00, 0x00 }, { 0x06, 0x01, 0x02, 0x04, 0x03, 0x00, 0x00 },
      { 0x07, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00 }, { 0x05, 0x05, 0x05, 0x05, 0x06, 0x00, 0x00 },
      { 0x05, 0x05, 0x05, 0x02, 0x02, 0x00, 0x00 }, { 0x05, 0x05, 0x07, 0x07, 0x02, 0x00, 0x00 },
      { 0x05, 0x05, 0x02, 0x05, 0x05, 0x00, 0x00 }, { 0x05, 0x05, 0x05, 0x02, 0x01, 0x00, 0x00 },
      { 0x07, 0x04, 0x02, 0x01, 0x07, 0x00, 0x00 }, { 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 0x00 },
      { 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00 }, { 0x07, 0x04, 0x04, 0x04, 0x07, 0x00, 0x00 },
      { 0x02, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00 },
      { 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x06, 0x05, 0x06, 0x00, 0x00 },
      { 0x01, 0x01, 0x03, 0x05, 0x03, 0x00, 0x00 }, { 0x00, 0x00, 0x06, 0x01, 0x06, 0x00, 0x00 },
      { 0x04, 0x04, 0x06, 0x05, 0x06, 0x00, 0x00 }, { 0x00, 0x00, 0x02, 0x07, 0x06, 0x00, 0x00 },
      { 0x02, 0x05, 0x01, 0x03, 0x01, 0x00, 0x00 }, { 0x00, 0x00, 0x06, 0x05, 0x06, 0x04, 0x02 },
      { 0x01, 0x01, 0x03, 0x05, 0x05, 0x00, 0x00 }, { 0x02, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00 },
      { 0x02, 0x00, 0x02, 0x02, 0x02, 0x02, 0x01 }, { 0x01, 0x01, 0x05, 0x03, 0x05, 0x00, 0x00 },
      { 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00 }, { 0x00, 0x00, 0x03, 0x07, 0x07, 0x00, 0x00 },
      { 0x00, 0x00, 0x03, 0x05, 0x05, 0x00, 0x00 }, { 0x00, 0x00, 0x02, 0x05, 0x02, 0x00, 0x00 },
      { 0x00, 0x00, 0x03, 0x05, 0x03, 0x01, 0x01 }, { 0x00, 0x00, 0x06, 0x05, 0x06, 0x04, 0x04 },
      { 0x00, 0x00, 0x07, 0x01, 0x01, 0x00, 0x00 }, { 0x00, 0x00, 0x06, 0x02, 0x03, 0x00, 0x00 },
      { 0x02, 0x02, 0x07, 0x02, 0x02, 0x00, 0x00 }, { 0x00, 0x00, 0x05, 0x05, 0x06, 0x00, 0x00 },
      { 0x00, 0x00, 0x05, 0x05, 0x02, 0x00, 0x00 }, { 0x00, 0x00, 0x05, 0x07, 0x02, 0x00, 0x00 },
      { 0x00, 0x00, 0x05, 0x02, 0x05, 0x00, 0x00 }, { 0x00, 0x00, 0x05, 0x05, 0x05, 0x02, 0x01 },
      { 0x00, 0x00, 0x07, 0x02, 0x07, 0x00, 0x00 }, { 0x04, 0x02, 0x03, 0x02, 0x04, 0x00, 0x00 },
      { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 }, { 0x01, 0x02, 0x06, 0x02, 0x01, 0x00, 0x00 },
      { 0x06, 0x83, 0x00, 0x80, 0x00, 0x80, 0x00 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }, { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x02, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00 },
      { 0x02, 0x07, 0x03, 0x07, 0x02, 0x00, 0x00 }, { 0x06, 0x02, 0x07, 0x02, 0x07, 0x00, 0x00 },
      { 0x00, 0x05, 0x02, 0x02, 0x05, 0x00, 0x00 }, { 0x05, 0x05, 0x02, 0x07, 0x02, 0x00, 0x00 },
      { 0x02, 0x02, 0x00, 0x02, 0x02, 0x00, 0x80 }, { 0x06, 0x01, 0x03, 0x06, 0x04, 0x02, 0x01 },
      { 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x07, 0x05, 0x07, 0x07, 0x05, 0x07, 0x00 },
      { 0x06, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x06, 0x03, 0x06, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x07, 0x04, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00 },
      { 0x02, 0x05, 0x07, 0x07, 0x05, 0x02, 0x00 }, { 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x02, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00 }, { 0x02, 0x02, 0x07, 0x02, 0x07, 0x00, 0x00 },
      { 0x03, 0x02, 0x01, 0x03, 0x00, 0x00, 0x00 }, { 0x03, 0x03, 0x02, 0x03, 0x00, 0x00, 0x00 },
      { 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x05, 0x05, 0x03, 0x01, 0x01 },
      { 0x06, 0x07, 0x07, 0x06, 0x06, 0x0E, 0x00 }, { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00 }, { 0x02, 0x03, 0x02, 0x07, 0x00, 0x00, 0x00 },
      { 0x02, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x03, 0x06, 0x03, 0x00, 0x00, 0x00 },
      { 0x01, 0x01, 0x05, 0x06, 0x07, 0x04, 0x00 }, { 0x01, 0x01, 0x07, 0x04, 0x02, 0x06, 0x00 },
      { 0x03, 0x03, 0x02, 0x07, 0x07, 0x04, 0x00 }, { 0x02, 0x00, 0x02, 0x01, 0x06, 0x00, 0x00 },
      { 0x02, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00 }, { 0x02, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00 },
      { 0x02, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00 }, { 0x02, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00 },
      { 0x05, 0x02, 0x05, 0x07, 0x05, 0x00, 0x00 }, { 0x02, 0x02, 0x05, 0x07, 0x05, 0x00, 0x00 },
      { 0x06, 0x03, 0x07, 0x03, 0x07, 0x00, 0x00 }, { 0x06, 0x01, 0x01, 0x01, 0x06, 0x00, 0x00 },
      { 0x07, 0x01, 0x03, 0x01, 0x07, 0x00, 0x00 }, { 0x07, 0x01, 0x03, 0x01, 0x07, 0x00, 0x00 },
      { 0x07, 0x01, 0x03, 0x01, 0x07, 0x00, 0x00 }, { 0x07, 0x01, 0x03, 0x01, 0x07, 0x00, 0x00 },
      { 0x07, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00 }, { 0x07, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00 },
      { 0x07, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00 }, { 0x07, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00 },
      { 0x03, 0x06, 0x07, 0x06, 0x03, 0x00, 0x00 }, { 0x07, 0x05, 0x07, 0x07, 0x05, 0x00, 0x00 },
      { 0x02, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00 }, { 0x02, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00 },
      { 0x02, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00 }, { 0x02, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00 },
      { 0x05, 0x02, 0x05, 0x05, 0x02, 0x00, 0x00 }, { 0x00, 0x00, 0x05, 0x02, 0x05, 0x00, 0x00 },
      { 0x06, 0x07, 0x05, 0x07, 0x03, 0x00, 0x00 }, { 0x05, 0x05, 0x05, 0x05, 0x06, 0x00, 0x00 },
      { 0x05, 0x05, 0x05, 0x05, 0x06, 0x00, 0x00 }, { 0x05, 0x05, 0x05, 0x05, 0x06, 0x00, 0x00 },
      { 0x05, 0x00, 0x05, 0x05, 0x06, 0x00, 0x00 }, { 0x05, 0x05, 0x02, 0x02, 0x02, 0x00, 0x00 },
      { 0x01, 0x07, 0x07, 0x01, 0x01, 0x00, 0x00 }, { 0x02, 0x07, 0x03, 0x07, 0x03, 0x00, 0x00 },
      { 0x01, 0x02, 0x06, 0x05, 0x06, 0x00, 0x00 }, { 0x02, 0x01, 0x06, 0x05, 0x06, 0x00, 0x00 },
      { 0x02, 0x05, 0x06, 0x05, 0x06, 0x00, 0x00 }, { 0x06, 0x03, 0x06, 0x05, 0x06, 0x00, 0x00 },
      { 0x05, 0x00, 0x06, 0x05, 0x06, 0x00, 0x00 }, { 0x02, 0x00, 0x06, 0x05, 0x06, 0x00, 0x00 },
      { 0x00, 0x00, 0x06, 0x07, 0x06, 0x00, 0x00 }, { 0x00, 0x00, 0x06, 0x01, 0x06, 0x00, 0x00 },
      { 0x01, 0x02, 0x02, 0x07, 0x06, 0x00, 0x00 }, { 0x02, 0x01, 0x02, 0x07, 0x06, 0x00, 0x00 },
      { 0x02, 0x05, 0x02, 0x07, 0x06, 0x00, 0x00 }, { 0x05, 0x00, 0x02, 0x07, 0x06, 0x00, 0x00 },
      { 0x01, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00 }, { 0x04, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00 },
      { 0x02, 0x05, 0x00, 0x02, 0x02, 0x00, 0x00 }, { 0x05, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00 },
      { 0x01, 0x02, 0x02, 0x05, 0x02, 0x00, 0x00 }, { 0x06, 0x01, 0x03, 0x05, 0x05, 0x00, 0x00 },
      { 0x01, 0x02, 0x02, 0x05, 0x02, 0x00, 0x00 }, { 0x04, 0x02, 0x02, 0x05, 0x02, 0x00, 0x00 },
      { 0x07, 0x00, 0x02, 0x05, 0x02, 0x00, 0x00 }, { 0x06, 0x03, 0x02, 0x05, 0x02, 0x00, 0x00 },
      { 0x05, 0x00, 0x02, 0x05, 0x02, 0x00, 0x00 }, { 0x02, 0x00, 0x07, 0x00, 0x02, 0x00, 0x00 },
      { 0x00, 0x00, 0x06, 0x05, 0x03, 0x00, 0x00 }, { 0x01, 0x02, 0x05, 0x05, 0x06, 0x00, 0x00 },
      { 0x04, 0x02, 0x05, 0x05, 0x06, 0x00, 0x00 }, { 0x07, 0x00, 0x05, 0x05, 0x06, 0x00, 0x00 },
      { 0x05, 0x00, 0x05, 0x05, 0x06, 0x00, 0x00 }, { 0x04, 0x02, 0x05, 0x05, 0x06, 0x02, 0x01 },
      { 0x01, 0x03, 0x05, 0x03, 0x01, 0x00, 0x00 }, { 0x05, 0x00, 0x05, 0x05, 0x05, 0x02, 0x01 },
    }}; // Font small
  } // namespace font
} // namespace dcled


