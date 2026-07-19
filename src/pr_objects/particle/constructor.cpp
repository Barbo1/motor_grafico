#include "../../../headers/pr_objects/particle.hpp"

Particle::Particle (Dir2 center, uint32_t radio, float mass) noexcept {
  this->position.store(center);
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->radio = radio;
  this->mass = mass;
}
