#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
void Impulse<IT, UT, F>::set_radio(float r) {
  this->radio = r;
}

template void Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::set_radio(float);
