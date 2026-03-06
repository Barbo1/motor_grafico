#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <cstring>

ImageModifier& ImageModifier::ymirror () {
  Uint32* arr = (Uint32*)this->texture->pixels;
  uint32_t h = this->texture->h;
  uint32_t w = this->texture->w;
  Uint32* aux = new Uint32[w];
  for (uint32_t i = 0; i < h/2; i++) {
    Uint32* dest = arr + (h - i - 1) * w;
    Uint32* src = arr + i * w;
    memcpy(aux, dest, w * sizeof(Uint32));
    memcpy(dest, src, w * sizeof(Uint32));
    memcpy(src, aux, w * sizeof(Uint32));
  }
  delete [] aux;
  return *this;
}
