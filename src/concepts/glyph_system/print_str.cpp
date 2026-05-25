#include "../../../headers/concepts/glyph_system.hpp"
#include <codecvt>
#include <cstdint>
#include <locale>

uint32_t GlyphsSystem::print (std::u16string str, uint16_t size, SDL_Color color, Dir2 position) {
  uint32_t w, h;
  float total_length = 0.f;
  float sizef = static_cast<float>(size);
  Uint32 colori = ((Uint32)color.r << 24) & ((Uint32)color.g << 16) & ((Uint32)color.b << 8) & (Uint32)color.a;
  for (const char16_t& character: str) {
    uint32_t key = GlyphsSystem::get_key(character, size, colori);
    auto founded = this->cached_glyphs.find(key);
    SDL_Texture* tex = nullptr;

    if (founded != this->cached_glyphs.end()) {
      tex = founded->second.tex;
      w = founded->second.w;
      h = founded->second.h;
    } else if (this->is_meta) {
      SDL_Surface* sur = this->raster (character, size, color);

      tex = SDL_CreateTextureFromSurface(glb->get_render(), sur);
      w = sur->w;
      h = sur->h;
      this->cached_glyphs.insert ({key, TTFCachedGlyphInfo {tex, w, h}});

      SDL_FreeSurface (sur);
    } else w = 0;

    if (tex != nullptr) {
      uint16_t pos = this->mapping[character];
      const ttf_glyph_data& data = this->glyphs[pos];

      SDL_Rect dst = SDL_Rect {
        .x = static_cast<int>(std::fmaf (data.left_bearing, sizef, position.x)),
        .y = static_cast<int>(std::lround(
          position.y - 
          sizef * (data.bounding_box.second.y + 0.1f) + 
          sizef * this->line_height * 0.25f
        )), 
        .w = static_cast<int>(w),
        .h = static_cast<int>(h)
      };
      SDL_RenderCopy (glb->get_render(), tex, nullptr, &dst);

      float coord = (
        pos < this->advance_widths.size () ? 
          this->advance_widths[pos] : 
          this->advance_widths.back()
      ) * sizef;

      total_length += coord;

      position += Dir2 (coord, 0.f);
    }
  }

  return total_length;
}

uint32_t GlyphsSystem::print (std::string str, uint16_t size, SDL_Color color, Dir2 position) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return this->print(conv.from_bytes(str), size, color, position);
}
