#include "../../../headers/pr_objects/nedge.hpp"

void NEdge::set_position (const AngDir2& center) {
  this->position.store(center);
  this->ang_pos = center.a();
  this->reposition_polygon();
}
