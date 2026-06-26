#include "../../../../headers/concepts/physical.hpp"

Physical::Physical () noexcept {
  this->glb = nullptr;
  this->position = MemDir2();
  this->velocity = MemDir2();
  this->force = MemDir2();
  this->area = 0.f;
  this->density = 0.f;
  this->f_k = 0.f;
  this->inertia = 0.f;
  this->config = 0;
}
