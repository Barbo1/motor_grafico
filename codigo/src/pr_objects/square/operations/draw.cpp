#include "../../../../headers/pr_objects/square.hpp"

void Square::draw (SDL_Renderer * render) {
  if (this->height != 0 && this->width != 0) {
    this->texture.draw (render, {this->position.x, this->position.y, 0});
  }
}
