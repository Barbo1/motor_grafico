#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
float Impulse<IT, UT_POSITION, F>::apply_coef (Circle cir) {
  AngDir2 diff = cir.get_position() - this->position;
  uint32_t radio = this->radio + cir.get_radio();
  float moddiff = diff.modulo();
  if (moddiff < radio) {
    float coef = this->generated_force_coef * moddiff;
    if constexpr (F == FT_QUADRATIC)
      return coef * coef;
    else if constexpr (F == FT_CUBIC)
      return coef * coef * coef;
    else if constexpr (F == FT_SINUSOIDAL)
      return std::sin(coef * 2.f * M_PI / radio);
    else if constexpr (F == FT_EXPONENTIAL)
      return std::exp(coef);
    else
      return coef;
  } else return 0.f;
}

template float Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::apply_coef(Circle); 
template float Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::apply_coef(Circle); 
template float Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::apply_coef(Circle); 
template float Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::apply_coef(Circle); 
template float Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::apply_coef(Circle); 
