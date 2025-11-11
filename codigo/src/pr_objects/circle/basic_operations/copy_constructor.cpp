#include "../../../../headers/pr_objects/circle.hpp"

Circle::Circle (const Circle & circle) noexcept : Physical (circle) {
  this->radio = circle.radio;
  this->texture = circle.texture;
  this->glb = circle.glb;
}
