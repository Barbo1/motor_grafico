#include "../../../../headers/pr_objects/particle.hpp"

void Particle::set_position(const Dir2 & position) {
  this->position.store(position);
}
