#include "../../../headers/pr_objects/nedge.hpp"

void NEdge::set_force (const AngDir2 & force) {
  this->force.store(force);
  this->ang_for = force.a();
}

void NEdge::add_force (const AngDir2 & force) {
  this->force.store(Dir2(force) + Dir2(this->force));
  this->ang_for += force.a();
}
