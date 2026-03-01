#include "../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

void GlyphsSystem::print (std::u16string str, uint16_t size, SDL_Color color, Dir2 position) {
  uint32_t w, h;
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
    }

    if (tex != nullptr) {
      uint16_t pos = this->mapping[character];
      const ttf_glyph_data& data = this->glyphs[pos];
      SDL_Rect dst;
      dst.x = std::fmaf (data.left_bearing, sizef, position.x);
      dst.y = std::lround(data.bounding_box.second.nmadd(sizef, position).y); 
      dst.w = w;
      dst.h = h;
      SDL_RenderCopyEx (glb->get_render(), tex, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);

      position += Dir2 {
        (pos < this->advance_widths.size () ? this->advance_widths[pos] : this->advance_widths.back()) * sizef, 
        0.f
      };
    }
  }
}
