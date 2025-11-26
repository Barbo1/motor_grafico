#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_POSITION, F>::set_height(float width) {
  this->dimension.x = width;
}

template void Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::set_height(float); 
template void Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::set_height(float);
template void Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::set_height(float);
template void Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::set_height(float);
template void Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::set_height(float);
