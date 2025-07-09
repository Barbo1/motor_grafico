#include "../../../../headers/concepts/texture.hpp"

#include <utility>

Texture & Texture::operator= (Texture && texture) {
  this->height = std::exchange(texture.height, 0);
  this->width = std::exchange(texture.width, 0);
  (*this->use_count)--;
  if ((*this->use_count) == 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
  this->use_count = std::exchange(texture.use_count, new int(0));
  this->texture = std::exchange(texture.texture, nullptr);
  return *this;
}
