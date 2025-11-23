#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
inline AngDir2 Impulse<IT_FAN, UT_POSITION, F>::apply(Circle& cir) {
  AngDir2 diff = cir.get_position() - this->position;
  AngDir2 diffa = (diff.abs() - this->dimension).max0();
  if (diffa.modulo2() < cir.get_radio() * cir.get_radio()) {
    float coef = (
      (diff * this->sign + this->dimension) * this->direction + cir.get_radio()
    ) * this->generated_force_coef;

    if constexpr (F == FT_QUADRATIC)
      return this->direction * (this->sign * coef * coef);
    else if constexpr (F == FT_CUBIC)
      return this->direction * (this->sign * coef * coef * coef);
    else if constexpr (F == FT_SINUSOIDAL)
      return this->direction * (this->sign * std::sin(
        M_PI * coef / (this->dimension * this->direction + cir.get_radio())
      ));
    else if constexpr (F == FT_EXPONENTIAL)
      return this->direction * (this->sign * std::exp(coef));
    else
      return this->direction * (this->sign * coef);
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::apply(Circle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::apply(Circle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::apply(Circle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::apply(Circle&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::apply(Circle&); 
