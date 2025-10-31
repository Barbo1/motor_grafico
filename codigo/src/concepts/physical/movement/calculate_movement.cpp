#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/primitives/operations.hpp"
#include <utility>

void Physical::calculate_movement(const AngDir2 & extrenal_forces) {
  if (this->_movible) {
    AngDir2 f = this->_force + extrenal_forces;
    if (std::exchange (this->_normal_presence, false)) {
      float a = min0 (f * this->_collition_normal);
      float b = this->_velocity * this->_collition_normal;
      AngDir2 vel = this->_collition_normal * b - this->_velocity;
      f += (-vel.normalize() * this->_acc_f_k + this->_collition_normal) * a;
    }
    float draw_coef = this->glb->get_time() * DRAW_RATE;
    f *= 20000 / (this->_density * this->_area);
    this->_velocity += f * draw_coef;
    this->position += this->_velocity * draw_coef;
  }
}
