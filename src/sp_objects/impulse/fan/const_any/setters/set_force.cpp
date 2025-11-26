#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
void Impulse<IT_FAN, UT, F>::set_force(AngDir2& force) {
  this->force = force;
}

template void Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::set_force(AngDir2&);
