#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
void Impulse<IT, UT, F>::set_position(AngDir2 position) {
  this->position = position;
}

template void Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::set_position(AngDir2);
