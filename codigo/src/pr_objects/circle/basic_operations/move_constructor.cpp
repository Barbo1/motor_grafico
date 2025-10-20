#include "../../../../headers/pr_objects/circle.hpp"
#include <utility>

Circle::Circle (Circle && circle) : Physical (circle) {
  this->glb = std::exchange(circle.glb, nullptr);
  this->radio = std::exchange(circle.radio, 0);
  this->texture = std::exchange(circle.texture, Visualizer<D2FIG>());
}
