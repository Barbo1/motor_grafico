#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/operations.hpp"

template <Function F>
float Impulse<IT_FAN, UT_POSITION, F>::apply_coef(Circle& cir) {
  Dir2 diff = cir.get_position() - this->position;
  Dir2 diffa = diff.abs() - this->dimension;
  diffa.x = max0 (diffa.x);
  diffa.y = max0 (diffa.y);
  if (diffa.modulo2() < cir.get_radio() * cir.get_radio()) {
    float coef = (
      (diff * this->sign + this->dimension) * this->direction + cir.get_radio()
    ) * this->generated_force_coef;

    if constexpr (F == FT_QUADRATIC)
      return coef * coef;
    else if constexpr (F == FT_CUBIC)
      return coef * coef * coef;
    else if constexpr (F == FT_SINUSOIDAL)
      return std::sin(M_PI * coef / (this->dimension * this->direction + cir.get_radio()));
    else if constexpr (F == FT_EXPONENTIAL)
      return std::exp(coef);
    else
      return coef;
  } else return 0.f;
}

template float Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::apply_coef(Circle&); 
template float Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::apply_coef(Circle&); 
template float Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::apply_coef(Circle&); 
template float Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::apply_coef(Circle&); 
template float Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::apply_coef(Circle&); 
