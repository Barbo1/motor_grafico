#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
float Impulse<IT_FAN, UT_VELOCITY, F>::get_width() {
  return this->dimension.x;
}

template float Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::get_width(); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::get_width();
template float Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::get_width();
template float Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::get_width();
template float Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::get_width();
