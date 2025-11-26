#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
float Impulse<IT_FAN, UT_VELOCITY, F>::get_gfcoef() {
  return this->generated_force_coef;
}

template float Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::get_gfcoef();
template float Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::get_gfcoef();
template float Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::get_gfcoef();
template float Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::get_gfcoef();
template float Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::get_gfcoef();
