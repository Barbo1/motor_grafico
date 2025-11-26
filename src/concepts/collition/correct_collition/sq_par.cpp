#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

void correct_collition (Particle& par, Square& sq) {
  AngDir2 diff = sq.get_position() - par.get_position();
  AngDir2 b = diff.bound(AngDir2 {sq.get_width(), sq.get_height(), 0.f}) - diff;
  par.add_position(b.normalize().msub(par.get_radio(), b));
}
