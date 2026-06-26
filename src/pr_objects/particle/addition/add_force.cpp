#include "../../../../headers/pr_objects/particle.hpp"

void Particle::add_force(const Dir2 & force) {
  this->force.store(force + Dir2(this->force));
}
