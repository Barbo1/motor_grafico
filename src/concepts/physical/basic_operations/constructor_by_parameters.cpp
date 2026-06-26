#include "../../../../headers/concepts/physical.hpp"

Physical::Physical (
    Global* glb, AngDir2 position, float density, float area, float inertia, float f_k, 
    bool movible
) noexcept {
  this->glb = glb;
  this->position.store(position);
  this->velocity.store(Dir2());
  this->force.store(Dir2());
  this->area = area;
  this->density = density;
  this->f_k = f_k;
  this->inertia = inertia;
  this->config = movible;
}
