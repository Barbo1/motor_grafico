#include "../../../../headers/concepts/texture.hpp"

Texture & Texture::operator= (const Texture & texture) {
  this->width = texture.width;
  this->height = texture.height;
  (*this->use_count)--;
  if ((*this->use_count) == 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
  this->texture = texture.texture;
  this->use_count = texture.use_count;
  return *this;
}
