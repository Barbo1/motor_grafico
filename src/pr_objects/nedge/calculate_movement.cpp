#include "../../../headers/pr_objects/nedge.hpp"
#include "../../../headers/primitives/types_definition.hpp"
#include "../../../headers/concepts/physical.hpp"

void NEdge::calculate_movement(Global* glb, const AngDir2 & extrenal_forces) {
  if (this->config & PCO_MOVIBLE) {
    AngDir2 final_force = AngDir2(this->force, this->ang_for) + extrenal_forces;
    AngDir2 velocity_2 = AngDir2(this->velocity, this->ang_vel);
    AngDir2 position_2 = AngDir2(this->position, this->ang_pos);

    if (this->config & PCO_IS_NORMAL) {
      this->config &= ~PCO_IS_NORMAL;
      AngDir2 collision_2 = AngDir2(this->collision_normal);
      float direction = final_force * collision_2;
      if (direction < 0.f) {
        float v_n = velocity_2 * collision_2;
        AngDir2 friction = collision_2.nmadd(v_n, velocity_2);

        final_force = AngDir2 ();
        if (friction.modulo2() > 0.001f)
          final_force = friction.normalize() * this->acc_f_k * direction;
      }
    }

    float coef_mult = (glb->get_time() + 1.f) * DRAW_RATE;

    final_force *= 2.f / (this->density * this->area);
    velocity_2 = final_force.madd(coef_mult, velocity_2);
    position_2 = velocity_2.madd (coef_mult, position_2);

    this->velocity.store(velocity_2);
    this->ang_vel = velocity_2.a();
    this->position.store(position_2);
    this->ang_pos = position_2.a();

    this->reposition_polygon();
  }
}
