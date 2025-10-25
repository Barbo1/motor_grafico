#include "../../../../headers/pr_objects/circle.hpp"

void Circle::draw () {
  if (this->radio != 0 && this->glb) {
    this->texture.draw(this->glb, this->position);
  }
}
