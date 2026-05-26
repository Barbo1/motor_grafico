#include "../../../headers/concepts/glyph_system.hpp"
#include <codecvt>
#include <cstdint>
#include <locale>

float GlyphsSystem::get_ascent (float size) {
  return this->ascent * size;
}

float GlyphsSystem::get_descent (float size) {
  return this->descent * size;
}
    
float GlyphsSystem::get_max_advance (float size) {
  return this->max_advance * size;
}

float GlyphsSystem::get_left_bearing(char16_t character, float size) {
  return this->glyphs[this->mapping[character]].left_bearing * size;
}

float GlyphsSystem::get_length (std::u16string str, uint32_t many, float size) {
  float total_length = 0.f;
  float sizef = static_cast<float>(size);
  for (uint32_t i = 0; i < many; i++) {
    const char16_t& character = str[i];
    uint32_t pos = this->mapping[character];
    total_length += (
      pos < this->advance_widths.size () ? 
        this->advance_widths[pos] : 
        this->advance_widths.back()
    ) * sizef;
  }

  return total_length;
}

float GlyphsSystem::get_length (std::string str, uint32_t many, float size) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return this->get_length(conv.from_bytes(str), many, size);
}
