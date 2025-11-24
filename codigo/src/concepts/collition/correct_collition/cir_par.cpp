#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

void correct_collition (Particle& par, Circle& cir) {
  par.set_position(
    (par.get_position() - cir.get_position())
      .normalize()
      .madd(par.get_radio() + cir.get_radio(), cir.get_position())
  );
}
