#include "../../../headers/pr_objects/circle.hpp"

Circle::Circle () noexcept {
  this->position.store(Dir2());
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->collision_normal.store(Dir2());
  this->radio = 0.f;
  this->area = 0.f;
  this->density = 0.f;
  this->f_k = 0.f;
  this->acc_f_k = 0.f;
  this->config = false;
}
