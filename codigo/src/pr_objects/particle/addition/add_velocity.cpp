#include "../../../../headers/pr_objects/particle.hpp"

void Particle::add_velocity(const AngDir2 & velocity) {
  this->_velocity += velocity;
}
