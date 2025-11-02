#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_POSITION, F>::set_width(float width) {
  this->dimension.y = width;
}

template void Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::set_width(float); 
template void Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::set_width(float);
template void Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::set_width(float);
template void Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::set_width(float);
template void Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::set_width(float);
