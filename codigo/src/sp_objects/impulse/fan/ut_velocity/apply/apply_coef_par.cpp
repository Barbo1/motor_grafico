#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/operations.hpp"

template <Function F>
float Impulse<IT_FAN, UT_VELOCITY, F>::apply_coef (Particle& par) {
  Dir2 diffa = (par.get_position() - this->position).abs() - this->dimension;
  diffa.x = max0 (diffa.x);
  diffa.y = max0 (diffa.y);
  if (diffa.modulo2() < par.get_radio() * par.get_radio()) {
    AngDir2 n = par.get_velocity();
    float coef = n.modulo() * this->generated_force_coef;
    
    /* return depending on the function. */
    if constexpr (F == FT_QUADRATIC)
      return coef * coef;
    else if constexpr (F == FT_CUBIC)
      return coef * coef * coef;
    else if constexpr (F == FT_EXPONENTIAL)
      return std::exp(coef);
    else
      return coef;
  } else return 0.f;
}

template float Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::apply_coef(Particle&); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::apply_coef(Particle&); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::apply_coef(Particle&); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::apply_coef(Particle&); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::apply_coef(Particle&); 
