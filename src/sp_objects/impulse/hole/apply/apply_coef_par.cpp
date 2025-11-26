#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
float Impulse<IT, UT, F>::apply_coef (Particle& par) {
  AngDir2 diff = par.get_position() - this->position;
  uint32_t radio = this->radio + par.get_radio();
  float moddiff = diff.modulo();
  if (moddiff < radio) {
    float coef = this->generated_force_coef * moddiff;
    if constexpr (F == FT_LINEAR)
      return coef;
    else if constexpr (F == FT_QUADRATIC)
      return coef * coef;
    else if constexpr (F == FT_CUBIC)
      return coef * coef * coef;
    else if constexpr (F == FT_SINUSOIDAL)
      return std::sin(coef * 2.f * M_PI / radio);
    else if constexpr (F == FT_EXPONENTIAL)
      return std::exp(coef);
    else
      return this->generated_force_coef;
  } else return 0.f;
}

template float Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::apply_coef(Particle&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::apply_coef(Particle&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::apply_coef(Particle&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::apply_coef(Particle&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::apply_coef(Particle&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::apply_coef(Particle&); 
