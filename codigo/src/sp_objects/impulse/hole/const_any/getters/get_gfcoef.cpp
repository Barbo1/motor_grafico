#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
float Impulse<IT, UT, F>::get_gfcoef() {
  return this->generated_force_coef;
}

template float Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::get_gfcoef();
