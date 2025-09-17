#include "../../../../headers/pr_objects/circle.hpp"

void Circle::draw (SDL_Renderer * render) {
  if (this->radio != 0) {
    this->texture.draw(render, this->position);
  }
}
