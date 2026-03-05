#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::operator- (const ImageModifier& img) {
  Uint32* arr_t = (Uint32*)this->texture->pixels;
  Uint32* arr_i = (Uint32*)img.texture->pixels;
  for (int i = 0; i < std::min(this->texture->h, img.texture->h); i++) {
    Uint32* res = arr_t + i * this->texture->w;
    for (int j = 0; j < std::min(this->texture->w, img.texture->w); j++, res++)
      *res = (*res & 0xFFFFFF00) | (255 - (arr_i[i * img.texture->w + j] & 0x000000FF));
  }
  return *this;
}
