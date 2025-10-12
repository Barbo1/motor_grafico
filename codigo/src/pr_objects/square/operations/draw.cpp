#include "../../../../headers/pr_objects/square.hpp"

void Square::draw (SDL_Renderer * render) {
  if (this->height != 0.f && this->width != 0.f) {
    this->texture.draw (render, {this->position.x, this->position.y, 0.f});
  }
}
