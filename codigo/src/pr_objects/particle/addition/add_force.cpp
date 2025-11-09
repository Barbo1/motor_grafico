#include "../../../../headers/pr_objects/particle.hpp"

void Particle::add_force(const AngDir2 & force) {
  this->_force += force;
}
