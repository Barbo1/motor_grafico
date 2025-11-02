#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
float Impulse<IT_FAN, UT_VELOCITY, F>::apply_coef (Square sq) {
  Dir2 diff = (this->position - sq.get_position()).abs();
  AngDir2 size = AngDir2 {sq.get_width(), sq.get_height(), 0} + this->dimension;
  AngDir2 n = AngDir2 ();
  if (diff.y < size.y && diff.x < size.x) {
    AngDir2 n = sq.get_velocity();
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

template float Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::apply_coef(Square); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::apply_coef(Square); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::apply_coef(Square); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::apply_coef(Square); 
template float Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::apply_coef(Square); 
