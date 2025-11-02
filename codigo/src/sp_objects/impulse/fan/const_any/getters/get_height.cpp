#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
float Impulse<IT_FAN, UT, F>::get_height() {
  return this->dimension.y;
}

template float Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::get_height();
