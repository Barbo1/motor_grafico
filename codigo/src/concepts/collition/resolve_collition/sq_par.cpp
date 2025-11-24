#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

void resolve_collition (Particle& par, Square& sq) {
  float mass_1 = sq.get_mass(), mass_2 = par.get_mass();
  AngDir2 diff = sq.get_position() - par.get_position();
  AngDir2 b = diff.bound(AngDir2 {sq.get_width(), sq.get_height(), 0.f}) - diff;
  AngDir2 n = b.normalize(); 
  float p = n * (sq.get_velocity() - par.get_velocity()) * 2.f / (mass_1 + mass_2);

  par.add_velocity(n * (p * mass_1));
  par.add_position(n.msub(par.get_radio(), b));
}
