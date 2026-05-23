#include "../../../headers/concepts/glyph_system.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <codecvt>
#include <locale>

void GlyphsSystem::fill (std::u16string str, uint16_t size, SDL_Color color, SDL_Texture* texture) {
  int w, h;
  SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
  Dir2 dims = Dir2(w, h);

  float sizef = static_cast<float>(size), total_width = 0.f;
  Uint32 colori = 
    ((Uint32)color.r << 24) & 
    ((Uint32)color.g << 16) & 
    ((Uint32)color.b << 8) & 
    (Uint32)color.a;
  uint32_t total_glyphs = 0, i = 0;

  while (i < str.size() && total_width < dims.x) {
    const char16_t& character = str[i];
    uint32_t key = GlyphsSystem::get_key(character, size, colori);
    auto founded = this->cached_glyphs.find(key);
    bool present = founded != this->cached_glyphs.end() || this->is_meta;

    if (this->is_meta) {
      SDL_Surface* sur = this->raster (character, size, color);
      SDL_Texture* tex = SDL_CreateTextureFromSurface(glb->get_render(), sur);
      uint32_t w = sur->w;
      uint32_t h = sur->h;
      SDL_FreeSurface (sur);
      this->cached_glyphs.insert ({key, TTFCachedGlyphInfo {tex, w, h}});
    }

    if (present) {
      uint16_t pos = this->mapping[character];
      total_glyphs++;
      total_width += (
        pos < this->advance_widths.size () ? 
          this->advance_widths[pos] : 
          this->advance_widths.back()
      ) * sizef;
    }

    i++;
  }

  SDL_Texture* actual_target = SDL_GetRenderTarget(glb->get_render());
  SDL_SetRenderTarget(glb->get_render(), texture);
  SDL_RenderClear(glb->get_render());

  if (total_glyphs == 0)
    return;
  else {
    float xposition = 0.f;

    uint32_t i = 0;
    while (i < str.size() && total_width < dims.x) {
      const char16_t& character = str[i];
      uint32_t key = GlyphsSystem::get_key(character, size, colori);
      auto founded = this->cached_glyphs.find(key);

      if (founded != this->cached_glyphs.end()) {
        uint16_t pos = this->mapping[character];
        const ttf_glyph_data& data = this->glyphs[pos];

        SDL_Rect dst = SDL_Rect {
          .x = static_cast<int>(std::fmaf (data.left_bearing, sizef, xposition)),
          .y = static_cast<int>(std::lround(
              dims.y * 0.5f - 
              sizef * (data.bounding_box.second.y + 0.1f) + 
              sizef * this->line_height * 0.25f
            )),
          .w = static_cast<int>(founded->second.w),
          .h = static_cast<int>(founded->second.h)
        };
        SDL_RenderCopy (glb->get_render(), founded->second.tex, nullptr, &dst);

        xposition += (
          pos < this->advance_widths.size () ? 
            this->advance_widths[pos] : 
            this->advance_widths.back()
        ) * sizef;
      }
      i++;
    }
  }

  SDL_SetRenderTarget(glb->get_render(), actual_target);
}

void GlyphsSystem::fill (std::string str, uint16_t size, SDL_Color color, SDL_Texture* texture) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  this->fill(conv.from_bytes(str), size, color, texture);
}
