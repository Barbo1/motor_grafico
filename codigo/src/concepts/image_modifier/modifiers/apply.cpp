#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::apply (Uint32 (*fun)(Uint32)) {
  Uint32* arr = (Uint32*)this->texture->pixels;
  for (int i = 0; i < this->texture->h * this->texture->w; i++) {
    arr[i] = fun(arr[i]);
  }
  return *this;
}
