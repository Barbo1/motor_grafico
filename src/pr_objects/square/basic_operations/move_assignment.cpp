#include "../../../../headers/pr_objects/square.hpp"
#include <utility>

Square & Square::operator= (Square && square) noexcept {
  this->Physical::operator=(square);
  this->texture = std::exchange (square.texture, Visualizer<D2FIG>());
  this->position = std::exchange (square.position, MemDir2());
  this->dims = std::exchange(square.dims, MemDir2());
  this->glb = std::exchange(square.glb, nullptr);
  return *this;
}
