#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
void Impulse<IT, UT, F>::set_gfcoef(float generated) {
  this->generated_force_coef = generated;
}

template void Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::set_gfcoef(float);
