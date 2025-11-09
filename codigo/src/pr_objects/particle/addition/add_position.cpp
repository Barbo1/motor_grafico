#include "../../../../headers/pr_objects/particle.hpp"

void Particle::add_position(const AngDir2 & position) {
  this->_position += position;
}
