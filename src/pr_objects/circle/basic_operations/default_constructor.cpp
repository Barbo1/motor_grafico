#include "../../../../headers/pr_objects/circle.hpp"

Circle::Circle () noexcept : Physical(), radio(0) {
  this->texture = Visualizer<D2FIG>();
  this->glb = nullptr;
}
