#include "../../../headers/concepts/glyph_system.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <codecvt>
#include <cstdint>
#include <locale>

uint32_t GlyphsSystem::get_right_window (std::u16string str, int32_t pos, uint16_t size, float dimention, float* bound) {
  const float sizef = static_cast<float>(size);
  const int strsizem1 = str.size() - 1;
  *bound = 0.f;
  if (strsizem1 < pos) 
    return strsizem1;
  float total_width = sizef * this->advance_widths[
    std::min(
      static_cast<int>(this->mapping[str[pos]]), 
      static_cast<int>(this->advance_widths.size() - 1)
    )
  ];
  while (pos < strsizem1 && total_width < dimention) {
    pos++;
    total_width += sizef * this->advance_widths[
      std::min(
        static_cast<int>(this->mapping[str[pos]]), 
        static_cast<int>(this->advance_widths.size() - 1)
      )
    ];
  }
  *bound = total_width;
  return pos + 1;
}

uint32_t GlyphsSystem::get_right_window (std::string str, int32_t pos, uint16_t size, float dimention, float* bound) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return this->get_right_window(conv.from_bytes(str), pos, size, dimention, bound);
}

uint32_t GlyphsSystem::get_left_window (std::u16string str, int32_t pos, uint16_t size, float dimention, float* bound) {
  const float sizef = static_cast<float>(size);
  *bound = 0.f;
  if (pos < 0) 
    return 0;
  float total_width = sizef * this->advance_widths[
    std::min(
      static_cast<std::size_t>(this->mapping[str[pos]]), 
      this->advance_widths.size()-1
    )
  ];
  while (0 < pos && total_width < dimention) {
    pos--;
    total_width += sizef * this->advance_widths[
      std::min(
        static_cast<std::size_t>(this->mapping[str[pos]]), 
        this->advance_widths.size()-1
      )
    ];
  }
  *bound = total_width;
  return pos;
}

uint32_t GlyphsSystem::get_left_window (std::string str, int32_t pos, uint16_t size, float dimention, float* bound) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return this->get_left_window(conv.from_bytes(str), pos, size, dimention, bound);
}
