#include "../../../../headers/pr_objects/square.hpp"

void Square::draw () {
  if (Dir2(this->dims) != Dir2() && this->glb) {
    this->texture.draw (this->glb, {this->position.x, this->position.y, 0.f});
  }
}
