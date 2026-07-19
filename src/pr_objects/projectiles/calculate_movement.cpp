#include "../../../headers/pr_objects/projectile.hpp"
#include "../../../headers/concepts/physical.hpp"
#include "../../../headers/primitives/types_definition.hpp"

void Projectile::calculate_movement(Global* glb, const AngDir2 & extrenal_forces) {
  if (this->config & PCO_MOVIBLE) {
    Dir2 final_force = Dir2(this->force) + Dir2(extrenal_forces);
    Dir2 velocity_2 = Dir2(this->velocity);

    if (this->config & PCO_IS_NORMAL) {
      this->config &= ~PCO_IS_NORMAL;
      Dir2 collision_2 = Dir2(this->collision_normal);
      float direction = final_force * collision_2;
      if (direction < 0.001f) {
        float v_n = velocity_2 * collision_2;
        Dir2 friction = collision_2.nmadd(v_n, velocity_2);

        final_force = AngDir2();
        if (friction.modulo2() > 0.001f)
          final_force = friction.normalize() * direction;
      }
    }

    float coef_mult = (glb->get_time() + 1.f) * DRAW_RATE;

    final_force *= 2.f / this->mass;
    Dir2 new_vel = final_force.madd(coef_mult, velocity_2);
    this->velocity.store(new_vel);
    this->position.store(new_vel.madd(coef_mult, Dir2(this->position)));
  }
}
