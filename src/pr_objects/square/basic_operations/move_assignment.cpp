#include "../../../../headers/pr_objects/square.hpp"
#include <utility>

Square & Square::operator= (Square && square) noexcept {
  this->Physical::operator=(square);
  this->texture = std::exchange (square.texture, Visualizer<D2FIG>());
  this->position = std::exchange (square.position, AngDir2{0.f, 0.f, 0.f});
  this->height = std::exchange (square.height, 0.f);
  this->width = std::exchange (square.width, 0.f);
  this->glb = std::exchange(square.glb, nullptr);
  return *this;
}
