#include "../../../../headers/pr_objects/particle.hpp"

Particle::Particle (const Particle & particle) noexcept {
  this->glb = particle.glb;
  this->_radio = particle._radio;
  this->_position = particle._position;
  this->_mass = particle._mass;
  this->_velocity = particle._velocity;
  this->_force = particle._force;
}
