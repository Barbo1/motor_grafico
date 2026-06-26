#include "../../../../headers/pr_objects/square.hpp"

Square::Square (const Square & square) noexcept : Physical(square) {
  this->position = square.position;
  this->texture = square.texture;
  this->dims = square.dims;
  this->glb = square.glb;
}
