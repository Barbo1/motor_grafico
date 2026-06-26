#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include <cmath>

void Physical::calculate_movement(const AngDir2 & extrenal_forces) {
  if (this->config & PCO_MOVIBLE) {
    Dir2 final_force = Dir2(this->force) + Dir2(extrenal_forces);
    Dir2 velocity_2 = Dir2(this->velocity);
    if (this->config & PCO_IS_NORMAL) {
      this->config &= ~PCO_IS_NORMAL;
      Dir2 collision_2 = Dir2(this->collision_normal);
      float direction = final_force * collision_2;
      if (direction < 0) {
        float v_n = velocity_2 * collision_2;
        Dir2 friction = (collision_2 * v_n) - velocity_2;

        if (friction.modulo2() > 0.001f)
          final_force = friction.normalize() * this->acc_f_k * -direction;
        else
          final_force = AngDir2 {0.f, 0.f, 0.f};
      }
    }

    float coef_mult = this->glb->get_time() * DRAW_RATE;

    final_force *= 20000.f / (this->density * this->area);
    Dir2 new_vel = final_force.madd(coef_mult, velocity_2);
    this->velocity.store(new_vel);
    this->position.store(new_vel.madd(coef_mult, Dir2(this->position)));
  }
}
