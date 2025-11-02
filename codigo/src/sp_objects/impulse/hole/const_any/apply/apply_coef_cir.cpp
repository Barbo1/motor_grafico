#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
float Impulse<IT, UT, F>::apply_coef (Circle cir) {
  AngDir2 diff = cir.get_position() - this->position;
  uint32_t radio = cir.get_radio() + this->radio;
  if (diff.modulo2() < radio * radio)
    return this->generated_force_coef;
  else return 0;
}

template float Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::apply_coef(Circle); 
