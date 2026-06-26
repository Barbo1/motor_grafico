#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
Dir2 Impulse<IT_FAN, UT, F>::get_position() {
  return this->position;
}

template Dir2 Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::get_position();
