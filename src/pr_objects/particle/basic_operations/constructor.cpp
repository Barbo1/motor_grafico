#include "../../../../headers/pr_objects/particle.hpp"

Particle::Particle (Global* glb, uint32_t radio, const Dir2& center, float mass) noexcept {
  this->glb = glb;
  this->position.store(center);
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->radio = radio;
  this->mass = mass;
}
