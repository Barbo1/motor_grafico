#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
AngDir2 Impulse<IT_FAN, UT_VELOCITY, F>::apply(Particle& cir) {
  AngDir2 diffa = ((cir.get_position() - this->position).abs() - this->dimension).max0();
  if (diffa.modulo2() < cir.get_radio() * cir.get_radio()) {
    AngDir2 n = cir.get_velocity();
    float coef = n.modulo() * this->generated_force_coef;
    
    /* return depending on the function. */
    if constexpr (F == FT_QUADRATIC)
      return n * (coef * this->generated_force_coef);
    else if constexpr (F == FT_CUBIC)
      return n * (coef * coef * this->generated_force_coef);
    else if constexpr (F == FT_EXPONENTIAL)
      return n.normalize() * std::exp(coef);
    else
      return n * this->generated_force_coef;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::apply(Particle&); 
