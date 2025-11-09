#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
void Impulse<IT_FAN, UT, F>::set_position(AngDir2& position) {
  this->position = position;
}

template void Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::set_position(AngDir2&);
