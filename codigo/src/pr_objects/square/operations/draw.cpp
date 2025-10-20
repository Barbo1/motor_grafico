#include "../../../../headers/pr_objects/square.hpp"

void Square::draw () {
  if (this->height != 0.f && this->width != 0.f && this->glb) {
    this->texture.draw (this->glb->get_render(), {this->position.x, this->position.y, 0.f});
  }
}
