#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
AngDir2 Impulse<IT, UT_POSITION, F>::apply(Circle cir) {
  AngDir2 diff = cir.get_position() - this->position;
  uint32_t radio = this->radio + cir.get_radio();
  float moddiff = diff.modulo();
  if (moddiff < radio) {
    float coef = this->generated_force_coef * moddiff;
    if constexpr (F == FT_QUADRATIC)
      return diff * this->generated_force_coef * coef;
    else if constexpr (F == FT_CUBIC)
      return diff * this->generated_force_coef * coef * coef;
    else if constexpr (F == FT_SINUSOIDAL)
      return diff.normalize() * std::sin(coef * 2.f * M_PI / radio);
    else if constexpr (F == FT_EXPONENTIAL)
      return diff.normalize() * std::exp(coef);
    else
      return diff * this->generated_force_coef;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::apply(Circle); 
