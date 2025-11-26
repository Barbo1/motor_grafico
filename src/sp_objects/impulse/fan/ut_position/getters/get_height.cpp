#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
float Impulse<IT_FAN, UT_POSITION, F>::get_height() {
  return this->dimension.y;
}

template float Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::get_height(); 
template float Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::get_height();
template float Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::get_height();
template float Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::get_height();
template float Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::get_height();
