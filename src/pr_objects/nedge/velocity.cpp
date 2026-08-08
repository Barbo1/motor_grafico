#include "../../../headers/pr_objects/nedge.hpp"

void NEdge::set_velocity (const AngDir2 & velocity) {
  this->velocity.store(velocity);
  this->ang_vel = velocity.a();
}

void NEdge::add_velocity (const AngDir2 & velocity) {
  this->velocity.store(Dir2(velocity) + Dir2(this->velocity));
  this->ang_vel += velocity.a();
}
