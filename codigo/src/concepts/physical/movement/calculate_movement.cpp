#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/primitives/operations.hpp"
#include <utility>

void Physical::calculate_movement(const AngDir2 & extrenal_forces) {
  if (this->_movible) {
    AngDir2 f = this->_force + extrenal_forces;
    if (std::exchange (this->_normal_presence, false)) {
      float ma = max0 (-f * this->_collition_normal);
      f += this->_collition_normal * ma;
      float b = this->_velocity * this->_collition_normal;
      AngDir2 vel = this->_collition_normal * b - this->_velocity;
      f += vel.normalize() * ma * this->_acc_f_k;
    }
    f *= MOVEMENT_COEFITIENT / (this->_density * this->_area);
    this->_velocity += f;
    this->position += this->_velocity * DRAW_RATE;
  }
}
