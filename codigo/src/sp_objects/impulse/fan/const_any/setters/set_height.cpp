#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
void Impulse<IT_FAN, UT, F>::set_height(float width) {
  this->dimension.y = width;
}

template void Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::set_height(float);
