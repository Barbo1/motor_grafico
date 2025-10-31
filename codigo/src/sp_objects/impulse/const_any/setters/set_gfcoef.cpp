#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, AngType AT, Function F>
void Impulse<IT, AT, F>::set_gfcoef(float generated) {
  this->generated_force_coef = generated;
}

template void Impulse<IT_HOLE, AT_NONE, FT_CONSTANT>::set_gfcoef(float);
