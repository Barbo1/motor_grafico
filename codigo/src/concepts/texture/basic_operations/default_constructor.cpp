#include "../../../../headers/concepts/texture.hpp"

Texture::Texture () {
  this->texture = nullptr;
  this->use_count = new int(0);
}
