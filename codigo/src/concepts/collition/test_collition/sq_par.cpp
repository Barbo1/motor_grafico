#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

bool test_collition (Particle& par, Square& sq) {
  Dir2 diff = (par.get_position() - sq.get_position()).abs();
  diff.x = max0 (diff.x - sq.get_width());
  diff.y = max0 (diff.y - sq.get_height());
  return diff.modulo2() < par.get_radio() * par.get_radio();
}
