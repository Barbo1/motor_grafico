#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_VELOCITY, F>::set_height(float width) {
  this->dimension.x = width;
}

template void Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::set_height(float); 
template void Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::set_height(float);
template void Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::set_height(float);
template void Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::set_height(float);
template void Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::set_height(float);
