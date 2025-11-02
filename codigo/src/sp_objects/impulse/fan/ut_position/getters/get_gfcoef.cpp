#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
float Impulse<IT_FAN, UT_POSITION, F>::get_gfcoef() {
  return this->generated_force_coef;
}

template float Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::get_gfcoef();
template float Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::get_gfcoef();
template float Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::get_gfcoef();
template float Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::get_gfcoef();
template float Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::get_gfcoef();
