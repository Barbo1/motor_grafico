#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/operations.hpp"

template <UnitType UT, Function F>
AngDir2 Impulse<IT_FAN, UT, F>::apply(Particle& par) {
  Dir2 diff = (par.get_position() - this->position).abs() - this->dimension;
  diff.x = max0 (diff.x);
  diff.y = max0 (diff.y);
  if (diff.modulo2() < par.get_radio() * par.get_radio()) {
    return this->force;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::apply(Particle&);
