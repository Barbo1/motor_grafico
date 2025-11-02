#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
float Impulse<IT_FAN, UT_VELOCITY, F>::get_height() {
  return this->dimension.y;
}

template float Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::get_height(); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::get_height();
template float Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::get_height();
template float Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::get_height();
template float Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::get_height();
