#include "../../../../headers/pr_objects/particle.hpp"
#include <utility>

Particle & Particle::operator= (Particle && particle) noexcept {
  this->glb = particle.glb;
  this->position = std::exchange(particle.position, MemDir2());
  this->velocity = std::exchange(particle.velocity, MemDir2());
  this->force = std::exchange(particle.force, MemDir2());
  this->radio = std::exchange(particle.radio, 0);
  this->mass = std::exchange(particle.mass, 0);
  return *this;
}
