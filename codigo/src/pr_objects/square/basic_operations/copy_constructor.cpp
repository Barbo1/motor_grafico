#include "../../../../headers/pr_objects/square.hpp"

Square::Square (const Square & square) noexcept : Physical(square) {
  this->position = square.position;
  this->texture = square.texture;
  this->height = square.height;
  this->width = square.width;
  this->glb = square.glb;
}
