#include "../../../headers/pr_objects/square.hpp"

Square::Square (AngDir2 center, uint32_t height, uint32_t width, float density, float f_k, bool movible) noexcept {
  this->position.store(center);
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->collision_normal.store(Dir2());
  this->dims.store(Dir2(width, height));
  this->area = 4.f * height * width;
  this->density = density;
  this->f_k = f_k;
  this->acc_f_k = 0.f;
  this->config = movible;
}
