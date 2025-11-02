#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
AngDir2 Impulse<IT_FAN, UT, F>::get_force() {
  return this->force;
}

template AngDir2 Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::get_force();
