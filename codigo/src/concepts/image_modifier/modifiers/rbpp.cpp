#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::rbpp (uint32_t number_reduction) {
  Uint32* arr = (Uint32*)this->texture->pixels;
  for (int i = 0; i < this->texture->h * this->texture->w; i++) {
    arr[i] &= ~((0x01010100 << number_reduction) - 0x01010100);
  }
  return *this;
}
