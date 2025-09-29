#include "../../../../headers/pr_objects/nedge.hpp"

void NEdge::draw (SDL_Renderer * render) {
  this->texture.draw(render, this->position);
}
