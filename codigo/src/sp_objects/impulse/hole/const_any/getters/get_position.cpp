#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
AngDir2 Impulse<IT, UT, F>::get_position() {
  return this->position;
}

template AngDir2 Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::get_position();
