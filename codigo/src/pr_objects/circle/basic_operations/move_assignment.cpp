#include "../../../../headers/pr_objects/circle.hpp"
#include <utility>

Circle & Circle::operator= (Circle && circle) {
  this->Physical::operator=(std::move(circle));
  this->radio = std::exchange(circle.radio, 0);
  this->texture = std::exchange(circle.texture, Visualizer<D2FIG>());
  return *this;
}
