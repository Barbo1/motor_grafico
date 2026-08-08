#include "../../../../headers/concepts/collision.hpp"

void correct_collision (Particle& par, Circle& cir) {
  Dir2 cir_pos = Dir2(cir.position);
  par.position.store(
    (Dir2(par.position) - cir_pos)
      .normalize()
      .madd(par.radio + cir.radio, cir_pos)
  );
}
