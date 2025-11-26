#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_VELOCITY, F>::set_width(float width) {
  this->dimension.y = width;
}

template void Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::set_width(float); 
template void Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::set_width(float);
template void Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::set_width(float);
template void Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::set_width(float);
template void Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::set_width(float);
