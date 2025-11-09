#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
void Impulse<IT, UT, F>::set_gfcoef(float generated) {
  this->generated_force_coef = generated;
}

template void Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::set_gfcoef(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::set_gfcoef(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::set_gfcoef(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::set_gfcoef(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::set_gfcoef(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::set_gfcoef(float);
