#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

bool test_collition (Particle& par, Square& sq) {
  AngDir2 diff = AngDir2 {
    (par.get_position() - sq.get_position()).abs() - AngDir2 {sq.get_width(), sq.get_height(), 0.f}
  }.max0();
  return diff.modulo2() < par.get_radio() * par.get_radio();
}
