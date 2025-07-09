#include "../../../../headers/concepts/texture.hpp"

Texture::Texture (const Texture & texture) {
  this->texture = texture.texture;
  this->width = texture.width;
  this->height = texture.height;
  this->use_count = texture.use_count;
  (*texture.use_count)++;
}
