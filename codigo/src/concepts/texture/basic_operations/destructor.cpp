#include "../../../../headers/concepts/texture.hpp"

Texture::~Texture() {
  (*this->use_count)--;
  if ((*this->use_count) == 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
}

