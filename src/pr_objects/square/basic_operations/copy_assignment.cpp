#include "../../../../headers/pr_objects/square.hpp"

Square & Square::operator= (const Square & square) noexcept {
  this->Physical::operator=(square);
  this->texture = square.texture;
  this->position = square.position;
  this->height = square.height;
  this->width = square.width;
  this->glb = square.glb;
  return *this;
}
