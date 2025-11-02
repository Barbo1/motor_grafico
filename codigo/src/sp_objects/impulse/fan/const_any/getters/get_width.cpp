#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
float Impulse<IT_FAN, UT, F>::get_width() {
  return this->dimension.x;
}

template float Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::get_width();
