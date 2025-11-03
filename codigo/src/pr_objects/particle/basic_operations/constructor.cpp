#include "../../../../headers/pr_objects/particle.hpp"

Particle::Particle (Global* glb, uint32_t radio, AngDir2 center, float mass) {
  this->glb = glb;
  this->_radio = radio;
  this->_position = center;
  this->_mass = mass;
  this->_velocity = AngDir2();
  this->_force = AngDir2();
}
