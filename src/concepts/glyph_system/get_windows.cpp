#include "../../../headers/concepts/glyph_system.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <codecvt>
#include <cstdint>
#include <locale>

uint32_t GlyphsSystem::get_right_window (std::u16string str, int32_t pos, uint16_t size, float dimention) {
  float total_width = 0.f, sizef = static_cast<float>(size);
  while (pos < static_cast<int>(str.size()) && total_width < dimention) {
    uint16_t j = this->mapping[str[pos]];
    total_width += sizef * (
      j < this->advance_widths.size () ? 
        this->advance_widths[j] : 
        this->advance_widths.back()
    );
    pos++;
  }
  return pos;
}

uint32_t GlyphsSystem::get_right_window (std::string str, int32_t pos, uint16_t size, float dimention) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return this->get_right_window(conv.from_bytes(str), pos, size, dimention);
}

uint32_t GlyphsSystem::get_left_window (std::u16string str, int32_t pos, uint16_t size, float dimention) {
  float total_width = 0.f, sizef = static_cast<float>(size);
  while (0 <= pos && total_width < dimention) {
    uint16_t j = this->mapping[str[pos]];
    total_width += sizef * (
      j < this->advance_widths.size () ? 
        this->advance_widths[j] : 
        this->advance_widths.back()
    );
    pos--;
  }
  return 0 <= pos ? pos : 0;
}

uint32_t GlyphsSystem::get_left_window (std::string str, int32_t pos, uint16_t size, float dimention) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return this->get_left_window(conv.from_bytes(str), pos, size, dimention);
}
