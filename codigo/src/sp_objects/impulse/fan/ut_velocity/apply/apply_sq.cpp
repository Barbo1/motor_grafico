#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
AngDir2 Impulse<IT_FAN, UT_VELOCITY, F>::apply(Square sq) {
  Dir2 diff = (this->position - sq.get_position()).abs();
  AngDir2 size = AngDir2 {sq.get_width(), sq.get_height(), 0} + this->dimension;
  AngDir2 n = AngDir2 ();
  if (diff.y < size.y && diff.x < size.x) {
    AngDir2 n = sq.get_velocity();
    float coef = n.modulo() * this->generated_force_coef;
    
    /* return depending on the function. */
    if constexpr (F == FT_QUADRATIC)
      return n * this->generated_force_coef * coef;
    else if constexpr (F == FT_CUBIC)
      return n * this->generated_force_coef * coef * coef;
    else if constexpr (F == FT_EXPONENTIAL)
      return n.normalize() * std::exp(coef);
    else 
      return n * this->generated_force_coef;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::apply(Square); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::apply(Square); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::apply(Square); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::apply(Square); 
template AngDir2 Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::apply(Square); 
