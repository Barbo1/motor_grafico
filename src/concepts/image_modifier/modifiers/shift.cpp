#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

ImageModifier& ImageModifier::shift (const Dir2& offset) {
  uint32_t w_new = static_cast<uint32_t>(offset.x);
  uint32_t h_new = static_cast<uint32_t>(offset.y);
  uint32_t h = this->texture->h;
  uint32_t w = this->texture->w;

  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, w + w_new, h + h_new, 32, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );

  if (sur != nullptr) {
    uint32_t i = 0;
    Uint32* arr = (Uint32*)sur->pixels;
    for (; i < h_new * (w + w_new); i++)
      arr[i] = 0;
    for (uint32_t k = 0; k < h; k++) {
      for (uint32_t j = 0; j < w_new; j++)
        arr[i++] = 0;
      for (uint32_t j = 0; j < w; j++)
        arr[i++] = ((Uint32*)this->texture->pixels)[k * w + j];
    }
    SDL_FreeSurface(this->texture);
    this->texture = sur;
  }
  return *this;
}
