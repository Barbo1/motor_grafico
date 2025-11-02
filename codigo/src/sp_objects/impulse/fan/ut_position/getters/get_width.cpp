#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
float Impulse<IT_FAN, UT_POSITION, F>::get_width() {
  return this->dimension.x;
}

template float Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::get_width(); 
template float Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::get_width();
template float Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::get_width();
template float Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::get_width();
template float Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::get_width();
