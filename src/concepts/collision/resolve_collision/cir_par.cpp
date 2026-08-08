#include "../../../../headers/concepts/collision.hpp"
#include "../../../../headers/concepts/physical.hpp"

void resolve_collision (Particle& par, Circle& cir) {
  float mass_1 = cir.get_mass();
  Dir2 par_pos = Dir2(par.position);
  Dir2 par_vel = Dir2(par.velocity);
  Dir2 n = (Dir2(cir.position) - par_pos).normalize();
  float p = (Dir2(cir.velocity) - par_vel) * n * 2.f / (mass_1 + par.mass);

  par.position.store(n.madd(cir.radio + par.radio, par_pos));
  par.velocity.store(n.madd(p * mass_1 * ENERGY_DISIPATION, par_vel));
}
