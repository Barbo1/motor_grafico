#include "../../../../../../headers/sp_objects/impulse.hpp"
#include <cmath>

template <Function F>
inline AngDir2 Impulse<IT_FAN, UT_POSITION, F>::apply(Square& sq) {
  AngDir2 diff = sq.get_position() - this->position;
  AngDir2 diffa = diff.abs();
  AngDir2 size = AngDir2 {sq.get_width(), sq.get_height(), 0} + this->dimension;
  if (diffa.y < size.y && diffa.x < size.x) {
    float coef = ((diff * this->sign + size) * this->direction) * this->generated_force_coef;

    /* return depending on the function. */
    if constexpr (F == FT_QUADRATIC)
      return this->direction * (this->sign * coef * coef);
    else if constexpr (F == FT_CUBIC)
      return this->direction * (this->sign * coef * coef * coef);
    else if constexpr (F == FT_SINUSOIDAL)
      return this->direction * (this->sign * std::sin(M_PI * coef / (size * this->direction)));
    else if constexpr (F == FT_EXPONENTIAL)
      return this->direction * (this->sign * std::exp(coef));
    else
      return this->direction * coef;
  } else return AngDir2 ();
}

template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::apply(Square&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::apply(Square&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::apply(Square&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::apply(Square&); 
template AngDir2 Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::apply(Square&); 
