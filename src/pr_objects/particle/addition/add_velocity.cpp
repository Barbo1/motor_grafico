#include "../../../../headers/pr_objects/particle.hpp"

void Particle::add_velocity(const Dir2 & velocity) {
  this->velocity.store(velocity + Dir2(this->velocity));
}
