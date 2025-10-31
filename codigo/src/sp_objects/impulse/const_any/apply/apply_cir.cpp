#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, AngType AT, Function F>
AngDir2 Impulse<IT, AT, F>::apply(Circle cir) {
  AngDir2 diff = cir.get_position() - this->position;
  uint32_t radio = cir.get_radio() + this->radio;
  if (diff.modulo2() < radio * radio)
    return diff.normalize() * this->generated_force_coef;
  else return AngDir2();
}

template AngDir2 Impulse<IT_HOLE, AT_NONE, FT_CONSTANT>::apply(Circle); 
