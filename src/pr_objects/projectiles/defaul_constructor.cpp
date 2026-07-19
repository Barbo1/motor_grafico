#include "../../../headers/pr_objects/projectile.hpp"

Projectile::Projectile () noexcept {
  this->position.store(Dir2());
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->collision_normal.store(Dir2());
  this->mass = 0.f;
  this->radio = 0.f;
  this->config = 0;
}
