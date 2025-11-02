#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_POSITION, F>::set_gfcoef(float generated) {
  this->generated_force_coef = generated;
}

template void Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::set_gfcoef(float);
template void Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::set_gfcoef(float);
template void Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::set_gfcoef(float);
template void Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::set_gfcoef(float);
template void Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::set_gfcoef(float);
