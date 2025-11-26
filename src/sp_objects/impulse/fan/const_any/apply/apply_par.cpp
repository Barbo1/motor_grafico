#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
AngDir2 Impulse<IT_FAN, UT, F>::apply(Particle& par) {
  AngDir2 diff = ((par.get_position() - this->position).abs() - this->dimension).max0();
  if (diff.modulo2() < par.get_radio() * par.get_radio()) {
    return this->force;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::apply(Particle&);
