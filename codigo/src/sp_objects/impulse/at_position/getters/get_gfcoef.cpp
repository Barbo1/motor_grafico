#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
float Impulse<IT, AT_POSITION, F>::get_gfcoef() {
  return this->generated_force_coef;
}

template float Impulse<IT_HOLE, AT_POSITION, FT_LINEAR>::get_gfcoef();
template float Impulse<IT_HOLE, AT_POSITION, FT_QUADRATIC>::get_gfcoef();
template float Impulse<IT_HOLE, AT_POSITION, FT_CUBIC>::get_gfcoef();
template float Impulse<IT_HOLE, AT_POSITION, FT_SINUSOIDAL>::get_gfcoef();
template float Impulse<IT_HOLE, AT_POSITION, FT_EXPONENTIAL>::get_gfcoef();
