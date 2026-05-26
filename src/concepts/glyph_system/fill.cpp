#include "../../../headers/concepts/glyph_system.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <codecvt>
#include <cstdint>
#include <locale>

float GlyphsSystem::fill (std::u16string_view str, uint16_t size, SDL_Color color, SDL_Texture* texture) {
  const float sizef = static_cast<float>(size);
  const Uint32 colori = 
    ((Uint32)color.r << 24) & 
    ((Uint32)color.g << 16) & 
    ((Uint32)color.b << 8) & 
    (Uint32)color.a;

  for (const char16_t& character: str) {
    uint32_t key = GlyphsSystem::get_key(character, size, colori);
    auto founded = this->cached_glyphs.find(key);

    if (founded == this->cached_glyphs.end() && this->is_meta) {
      SDL_Surface* sur = this->raster (character, size, color);
      SDL_Texture* tex = SDL_CreateTextureFromSurface(glb->get_render(), sur);
      uint32_t w = sur->w;
      uint32_t h = sur->h;
      SDL_FreeSurface (sur);
      this->cached_glyphs.insert ({key, TTFCachedGlyphInfo {tex, w, h}});
    }
  }

  int h;
  SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &h);
  const float dimy2 = static_cast<float>(h) * 0.5f;

  float xposition = 0.f;

  SDL_Texture* actual_target = SDL_GetRenderTarget(glb->get_render());
  SDL_SetRenderTarget(glb->get_render(), texture);
  SDL_RenderClear(glb->get_render());

  for (const char16_t& character: str) {
    uint32_t key = GlyphsSystem::get_key(character, size, colori);
    auto founded = this->cached_glyphs.find(key);

    if (founded != this->cached_glyphs.end()) {
      int pos = this->mapping[character];
      const ttf_glyph_data& data = this->glyphs[pos];

      SDL_Rect dst = SDL_Rect {
        .x = static_cast<int>(std::fmaf(data.left_bearing, sizef, xposition)),
        .y = static_cast<int>(std::round(
          dimy2 - 
          sizef * (data.bounding_box.second.y + 0.1f) + 
          sizef * this->line_height * 0.25f
        )),
        .w = static_cast<int>(founded->second.w),
        .h = static_cast<int>(founded->second.h)
      };
      SDL_RenderCopy(glb->get_render(), founded->second.tex, nullptr, &dst);

      xposition += sizef * this->advance_widths[
        std::min(pos, static_cast<int>(this->advance_widths.size()-1))
      ];
    }
  }

  SDL_SetRenderTarget(glb->get_render(), actual_target);

  return xposition;
}

float GlyphsSystem::fill (std::string_view str, uint16_t size, SDL_Color color, SDL_Texture* texture) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return this->fill(conv.from_bytes(std::string(str)), size, color, texture);
}
