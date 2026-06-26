#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
Dir2 Impulse<IT_FAN, UT_POSITION, F>::get_position() {
  return this->position;
}

template Dir2 Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::get_position(); 
template Dir2 Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::get_position();
template Dir2 Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::get_position();
template Dir2 Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::get_position();
template Dir2 Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::get_position();
