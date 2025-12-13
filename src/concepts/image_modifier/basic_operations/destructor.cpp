#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier::~ImageModifier () {
  if (this->texture != nullptr) {
    if (this->aquired)
      std::free (this->texture->pixels);
    SDL_FreeSurface (this->texture);
  }
}

