#include "../../../../headers/concepts/image_modifier.hpp"
#include <cmath>

ImageModifier& ImageModifier::operator* (const ImageModifier& img) {
  Uint32* arr_this = (Uint32*)this->texture->pixels;
  Uint32* arr_img = (Uint32*)img.texture->pixels;

  uint32_t h = std::min (this->texture->h, img.texture->h);
  uint32_t w = std::min (this->texture->h, img.texture->h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      arr_this[i * this->texture->w + j] = arr_img[i * img.texture->w + j];
    }
  }
  return *this;
}


