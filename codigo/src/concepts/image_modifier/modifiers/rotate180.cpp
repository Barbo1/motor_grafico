#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::rotate180 () {
  Uint32* arr = (Uint32*)this->texture->pixels;
  uint32_t h = this->texture->h;
  uint32_t w = this->texture->w;
  for (int i = 0; i < h/2; i++) {
    for (int j = 0; j < w; j++) {
      std::swap(
        arr[(h - i - 1) * w + (w - 1 - j)], 
        arr[i * w + j]
      );
    }
  }
  return *this;
}
