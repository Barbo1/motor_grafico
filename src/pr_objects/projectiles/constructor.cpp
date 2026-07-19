#include "../../../headers/pr_objects/projectile.hpp"

Projectile::Projectile (AngDir2 center, float radio, float mass, bool movible) noexcept {
  this->position.store(center);
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->collision_normal.store(Dir2());
  this->mass = mass;
  this->radio = radio;
  this->config = movible;
}
