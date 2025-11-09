#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
float Impulse<IT, UT, F>::get_gfcoef() {
  return this->generated_force_coef;
}

template float Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::get_gfcoef();
template float Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::get_gfcoef();
template float Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::get_gfcoef();
template float Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::get_gfcoef();
template float Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::get_gfcoef();
template float Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::get_gfcoef();
