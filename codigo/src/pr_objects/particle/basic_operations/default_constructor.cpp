#include "../../../../headers/pr_objects/particle.hpp"

Particle::Particle () {
  this->glb = nullptr;
  this->_radio = 0;
  this->_position = AngDir2();
  this->_mass = 0;
  this->_velocity = AngDir2();
  this->_force = AngDir2();
}
