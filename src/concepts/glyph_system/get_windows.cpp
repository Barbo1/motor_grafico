#include "../../../headers/concepts/glyph_system.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <codecvt>
#include <cstdint>
#include <locale>

void GlyphsSystem::get_right_window (std::u16string str, int32_t pos, uint16_t size, float dimention, uint32_t* window_bound, float* dev) {
  uint32_t i = pos;
  float total_width = 0.f, sizef = static_cast<float>(size);
  while (i < str.size() && total_width < dimention) {
    uint16_t j = this->mapping[str[i]];
    total_width += (
      j < this->advance_widths.size () ? 
        this->advance_widths[j] : 
        this->advance_widths.back()
    ) * sizef;
    i++;
  }
  *window_bound = i;
  *dev = total_width - dimention;
}

void GlyphsSystem::get_right_window (std::string str, int32_t pos, uint16_t size, float dimention, uint32_t* window_bound, float* dev) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  this->get_right_window(conv.from_bytes(str), pos, size, dimention, window_bound, dev);
}

void GlyphsSystem::get_left_window (std::u16string str, int32_t pos, uint16_t size, float dimention, uint32_t* window_bound, float* dev) {
  int32_t i = pos;
  float total_width = 0.f, sizef = static_cast<float>(size);
  while (-1 < i && total_width < dimention) {
    uint16_t j = this->mapping[str[i]];
    total_width += (
      j < this->advance_widths.size () ? 
        this->advance_widths[j] : 
        this->advance_widths.back()
    ) * sizef;
    i--;
  }
  *window_bound = i;
  *dev = total_width - dimention;
}

void GlyphsSystem::get_left_window (std::string str, int32_t pos, uint16_t size, float dimention, uint32_t* window_bound, float* dev) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  this->get_left_window(conv.from_bytes(str), pos, size, dimention, window_bound, dev);
}
