#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

bool test_collition (const Particle& par, const Square& sq) {
  AngDir2 diff = AngDir2 {
    (par._position - sq.position).abs() - AngDir2 {sq.width, sq.height, 0.f}
  }.max0();
  return diff.modulo2() < par._radio * par._radio;
}
