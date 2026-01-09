#include "../../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

void GlyphsSystem::print (std::u16string str, uint32_t size, Dir2 position) {
  uint32_t w, h;
  SDL_Texture* tex;
  float sizef = static_cast<float>(size);
  for (const char16_t& character: str) {
    uint32_t key = (size && character ? size << 15 | character : 0);
    auto founded = this->cached_glyphs.find(key);

    if (founded != this->cached_glyphs.end()) {
      tex = founded->second.tex;
      w = founded->second.w;
      h = founded->second.h;
    } else {
      SDL_Surface* sur = this->raster (character, size);
      if (sur == nullptr)
        continue;

      tex = SDL_CreateTextureFromSurface(glb->get_render(), sur);
      w = sur->w;
      h = sur->h;
      this->cached_glyphs.insert ({key, TTFCachedGlyphInfo {tex, w, h}});

      delete [] (Uint32*)sur->pixels;
      SDL_FreeSurface (sur);
    }
    
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
