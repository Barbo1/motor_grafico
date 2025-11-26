#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

ImageModifier& ImageModifier::ymirror () {
  Uint32* arr = (Uint32*)this->texture->pixels;
  uint32_t h = this->texture->h;
  uint32_t w = this->texture->w;
  for (uint32_t i = 0; i < h/2; i++) {
    for (uint32_t j = 0; j < w; j++) {
      std::swap(
        arr[(h - i - 1) * w + j], 
        arr[i * w + j]
      );
    }
  }
  return *this;
}
