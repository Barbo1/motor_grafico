#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::xmirror () {
  Uint32* arr = (Uint32*)this->texture->pixels;
  uint32_t h = this->texture->h;
  uint32_t w = this->texture->w;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w/2; j++) {
      std::swap(
        arr[i * w + j], 
        arr[(i + 1) * w - 1 - j]
      );
    }
  }
  return *this;
}
