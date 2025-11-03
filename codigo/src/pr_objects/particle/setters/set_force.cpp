#include "../../../../headers/pr_objects/particle.hpp"

void Particle::set_force(const AngDir2 & force) {
  this->_force = force;
}
