#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier::~ImageModifier () {
  if (this->texture != nullptr) {
    SDL_FreeSurface (this->texture);
  }
}

