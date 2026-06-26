#include "../../../../headers/pr_objects/particle.hpp"

void Particle::set_velocity(const Dir2 & velocity) {
  this->velocity.store(velocity);
}
