#include "../../../../../../headers/sp_objects/impulse.hpp"
#include <cmath>

template <Function F>
inline float Impulse<IT_FAN, UT_POSITION, F>::apply_coef(Square& sq) {
  AngDir2 diff = sq.get_position() - this->position;
  AngDir2 diffa = diff.abs();
  AngDir2 size = AngDir2 {sq.get_width(), sq.get_height(), 0} + this->dimension;
  if (diffa.y < size.y && diffa.x < size.x) {
    float coef = ((diff * this->sign + size) * this->direction) * this->generated_force_coef;

    /* return depending on the function. */
    if constexpr (F == FT_QUADRATIC)
      return coef * coef;
    else if constexpr (F == FT_CUBIC)
      return coef * coef * coef;
    else if constexpr (F == FT_SINUSOIDAL)
      return std::sin(M_PI * coef / (size * this->direction));
    else if constexpr (F == FT_EXPONENTIAL)
      return std::exp(coef);
    else
      return coef;
  } else return 0.f;
}

template float Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::apply_coef(Square&); 
template float Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::apply_coef(Square&); 
template float Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::apply_coef(Square&); 
template float Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::apply_coef(Square&); 
template float Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::apply_coef(Square&); 
