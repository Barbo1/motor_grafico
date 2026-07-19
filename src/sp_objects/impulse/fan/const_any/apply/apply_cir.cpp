#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
AngDir2 Impulse<IT_FAN, UT, F>::apply(Circle& cir) {
  Dir2 diff = ((Dir2(cir.position) - Dir2(this->position)).abs() - this->dimension).max0();
  if (diff.modulo2() < cir.radio * cir.radio) {
    return this->force;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::apply(Circle&); 
