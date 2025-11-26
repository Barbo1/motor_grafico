#include "../../../../headers/concepts/physical.hpp"
#include <cmath>

void Physical::calculate_movement(const AngDir2 & extrenal_forces) {
  if (this->_movible) {
    AngDir2 f = this->_force + extrenal_forces;
    if (this->_normal_presence) {
      this->_normal_presence = false;

      float a = std::fmin (0.f, f * this->_collition_normal);
      float b = this->_velocity * this->_collition_normal;

      f += this->_collition_normal
        .msub(b, this->_velocity)
        .normalize()
        .nmadd(this->_acc_f_k, this->_collition_normal) * a;
    }
    AngDir2 coef_mult(this->glb->get_time() * DRAW_RATE);
    f *= 20000.f / (this->_density * this->_area);
    this->_velocity += f.dir_mul(coef_mult);
    this->position += this->_velocity.dir_mul(coef_mult);
  }
}
