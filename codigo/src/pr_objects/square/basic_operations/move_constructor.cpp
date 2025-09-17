#include "../../../../headers/pr_objects/square.hpp"
#include <utility>

Square::Square (Square && square): Physical(square) {
  this->texture = std::exchange (square.texture, Visualizer<D2FIG>());
  this->position = std::exchange (square.position, {0,0,0});
  this->height = std::exchange (square.height, 0);
  this->width = std::exchange (square.width, 0);
}
