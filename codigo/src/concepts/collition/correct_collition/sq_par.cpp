#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void correct_collition (Particle& par, Square& sq) {
  float mass_1 = sq.get_mass(), mass_2 = par.get_mass();
  AngDir2 diff = sq.get_position() - par.get_position();
  AngDir2 b = AngDir2 {
    bound (diff.x, sq.get_width()) - diff.x,
    bound (diff.y, sq.get_height()) - diff.y,
    0
  };
  AngDir2 n = b.normalize(); 
  par.set_position(par.get_position() - b + n * par.get_radio());
}
