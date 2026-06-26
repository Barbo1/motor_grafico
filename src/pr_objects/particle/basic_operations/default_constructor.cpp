#include "../../../../headers/pr_objects/particle.hpp"

Particle::Particle () noexcept {
  this->glb = nullptr;
  this->position.store(Dir2());
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->radio = 0;
  this->mass = 0;
}
