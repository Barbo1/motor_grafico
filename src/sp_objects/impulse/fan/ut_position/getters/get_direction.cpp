#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
AngDir2 Impulse<IT_FAN, UT_POSITION, F>::get_direction() {
  return this->direction;
}

template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::get_direction(); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::get_direction();
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::get_direction();
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::get_direction();
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::get_direction();
