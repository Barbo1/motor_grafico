#include "../../../../headers/pr_objects/circle.hpp"

Circle::Circle (const Circle & circle) : Physical (circle) {
  this->radio = circle.radio;
  this->texture = circle.texture;
}
