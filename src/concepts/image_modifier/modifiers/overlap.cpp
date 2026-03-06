#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>

ImageModifier& ImageModifier::operator* (const ImageModifier& img) {
  Uint32* arr_t = (Uint32*)this->texture->pixels;
  Uint32* arr_i = (Uint32*)img.texture->pixels;

  for (int i = 0; i < std::min(this->texture->h, img.texture->h); i++) {
    Uint32* res_t = arr_t + i * this->texture->w;
    Uint32* res_i = arr_i + i * img.texture->w ;
    for (int j = 0; j < std::min(this->texture->w, img.texture->w); j++, res_t++, res_i++) {
      Uint8 a2 = *res_t & 0xFF;
      if (a2 != 0)
        *res_t = (*res_i & 0xFFFFFF00) | a2;
    }
  }

  return *this;
}
