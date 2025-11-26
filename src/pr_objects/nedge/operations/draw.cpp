#include "../../../../headers/pr_objects/nedge.hpp"

void NEdge::draw () {
  this->texture.draw(glb, this->position);
}
