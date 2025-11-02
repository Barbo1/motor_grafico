#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_VELOCITY, F>::set_position(AngDir2 position) {
  this->position = position;
}

template void Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::set_position(AngDir2); 
template void Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::set_position(AngDir2);
template void Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::set_position(AngDir2);
template void Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::set_position(AngDir2);
template void Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::set_position(AngDir2);
