#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

ImageModifier& ImageModifier::shift (const Dir2& offset) {
  uint32_t w_off = static_cast<uint32_t>(offset.x);
  uint32_t h_off = static_cast<uint32_t>(offset.y);
  uint32_t h = this->texture->h;
  uint32_t w = this->texture->w;

  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, w + w_off, h + h_off, 32, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );

  if (sur != nullptr) {
    Uint32* arr = (Uint32*)sur->pixels + h_off * (w + w_off) + w_off;
    Uint32* src = (Uint32*)this->texture->pixels;
    for (uint32_t k = 0; k < h; k++, src += w, arr += w + w_off)
      memcpy (arr, src, sizeof(uint32_t) * w);

    if (this->aquired)
      std::free(this->texture->pixels);
    SDL_FreeSurface(this->texture);

    this->aquired = 0;
    this->texture = sur;
  }
  return *this;
}
