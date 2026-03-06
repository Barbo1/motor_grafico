#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>

ImageModifier& ImageModifier::xshear (float angle) {
  float m = 1 / std::tan (angle);

  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, this->texture->w + static_cast<uint32_t>(this->texture->h * m), this->texture->h, 32, 
    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );

  Uint32* fin = (Uint32*)sur->pixels;
  Uint32* arr = (Uint32*)this->texture->pixels;
  float i = 0.f, top = static_cast<float>(this->texture->h);
  for (uint32_t rowa = 0, j = 0; i < top; i+=1.f, rowa += this->texture->w, j += sur->w) {
    uint32_t n = j + static_cast<uint32_t>(i * m);
    memcpy (fin + n, arr + rowa, this->texture->w * sizeof(Uint32));
  }

  if (this->aquired)
    std::free(this->texture->pixels);
  SDL_FreeSurface (this->texture);

  this->texture = sur;
  this->aquired = 0;

  return *this;
}
