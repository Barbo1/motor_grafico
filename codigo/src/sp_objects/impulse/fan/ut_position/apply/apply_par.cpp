#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/operations.hpp"

template <Function F>
inline AngDir2 Impulse<IT_FAN, UT_POSITION, F>::apply(Particle& par) {
  Dir2 diff = par.get_position() - this->position;
  Dir2 diffa = diff.abs() - this->dimension;
  diffa.x = max0 (diffa.x);
  diffa.y = max0 (diffa.y);
  if (diffa.modulo2() < par.get_radio() * par.get_radio()) {
    float coef = (
      (diff * this->sign + this->dimension) * this->direction + par.get_radio()
    ) * this->generated_force_coef;

    if constexpr (F == FT_QUADRATIC)
      return this->direction * (this->sign * coef * coef);
    else if constexpr (F == FT_CUBIC)
      return this->direction * (this->sign * coef * coef * coef);
    else if constexpr (F == FT_SINUSOIDAL)
      return this->direction * (this->sign * std::sin(
        M_PI * coef / (this->dimension * this->direction + par.get_radio())
      ));
    else if constexpr (F == FT_EXPONENTIAL)
      return this->direction * (this->sign * std::exp(coef));
    else
      return this->direction * (this->sign * coef);
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::apply(Particle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::apply(Particle&); 
