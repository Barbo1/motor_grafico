#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier::~ImageModifier () {
  if (this->texture != nullptr) {
    if (this->aquired)
      delete [] (Uint32*)this->texture->pixels;
    SDL_FreeSurface (this->texture);
  }
}

