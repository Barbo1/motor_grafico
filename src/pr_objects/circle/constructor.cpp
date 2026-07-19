#include "../../../headers/pr_objects/circle.hpp"

Circle::Circle (Dir2 center, float radio, float density, float f_k, bool movible) noexcept {
  this->position.store(center);
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->collision_normal.store(Dir2());
  this->radio = radio;
  this->area = M_PI * radio * radio;
  this->density = density;
  this->f_k = f_k;
  this->acc_f_k = 0.f;
  this->config = movible;
}

