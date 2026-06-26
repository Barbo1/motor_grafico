#include "../../../../headers/pr_objects/particle.hpp"

void Particle::set_force(const Dir2 & force) {
  this->force.store(force);
}
