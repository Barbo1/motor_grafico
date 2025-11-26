#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
AngDir2 Impulse<IT_FAN, UT_VELOCITY, F>::get_position() {
  return this->position;
}

template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::get_position(); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::get_position();
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::get_position();
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::get_position();
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::get_position();
