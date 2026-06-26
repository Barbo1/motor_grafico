#include "../../../../headers/pr_objects/particle.hpp"

Particle & Particle::operator= (const Particle & particle) noexcept {
  this->glb = particle.glb;
  this->position = particle.position;
  this->velocity = particle.velocity;
  this->force = particle.force;
  this->radio = particle.radio;
  this->mass = particle.mass;
  return *this;
}
