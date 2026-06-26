#include "../../../../headers/pr_objects/particle.hpp"

void Particle::add_position(const Dir2 & position) {
  this->position.store(position + Dir2(this->position));
}
