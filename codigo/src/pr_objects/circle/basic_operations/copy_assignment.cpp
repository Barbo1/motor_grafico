#include "../../../../headers/pr_objects/circle.hpp"

Circle & Circle::operator= (const Circle & circle) {
  this->Physical::operator=(circle);
  this->texture = circle.texture;
  this->radio = circle.radio;
  this->glb = circle.glb;
  return *this;
}
