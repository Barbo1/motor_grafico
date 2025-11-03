#include "../../../../headers/pr_objects/particle.hpp"
#include <utility>

Particle::Particle(Particle && particle) {
  this->glb = particle.glb;
  this->_radio = std::exchange(particle._radio, 0);
  this->_position = std::exchange(particle._position, AngDir2());
  this->_mass = std::exchange(particle._mass, 0);
  this->_velocity = std::exchange(particle._velocity, AngDir2());
  this->_force = std::exchange(particle._force, AngDir2());
}
