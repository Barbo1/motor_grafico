#include "../../../../headers/concepts/physical.hpp"
#include <cmath>

void Physical::calculate_movement(const AngDir2 & extrenal_forces) {
  if (this->_movible) {
    AngDir2 final_force = this->_force + extrenal_forces;
    if (this->_normal_presence) {
      this->_normal_presence = false;

      float direction = final_force * this->_collition_normal;
      if (direction < 0) {
        final_force = AngDir2 {0.f, 0.f, 0.f};
        float v_n = this->_velocity * this->_collition_normal;
        AngDir2 friction = (this->_collition_normal * v_n) - this->_velocity;
        if (friction.modulo2() > 0.0001)
          final_force = friction
            .normalize()
            .nmadd(this->_acc_f_k, this->_collition_normal)
            .madd(direction, final_force);
      }
    }

    AngDir2 coef_mult(this->glb->get_time() * DRAW_RATE);

    final_force *= 20000.f / (this->_density * this->_area);
    this->_velocity += final_force.dir_mul (coef_mult);
    this->position += this->_velocity.dir_mul (coef_mult);
  }
}
