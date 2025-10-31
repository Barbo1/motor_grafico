#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
AngDir2 Impulse<IT, AT_POSITION, F>::apply(Circle cir) {
  AngDir2 diff = cir.get_position() - this->position;
  uint32_t radio = this->radio + cir.get_radio();
  if (diff.modulo2() < radio * radio) {
    AngDir2 n = diff.normalize();
    
    /* return depending on the function. */
    if constexpr (F == FT_QUADRATIC)
      return n.normalize() * this->generated_force_coef * n.modulo2();
    else if constexpr (F == FT_CUBIC)
      return n * this->generated_force_coef * n.modulo2();
    else if constexpr (F == FT_SINUSOIDAL)
      return n.normalize() * this->generated_force_coef * std::sin(n.modulo());
    else if constexpr (F == FT_EXPONENTIAL)
      return n.normalize() * std::exp(n.modulo() * this->generated_force_coef);
    else
      return n * this->generated_force_coef;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_LINEAR>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_QUADRATIC>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_CUBIC>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_SINUSOIDAL>::apply(Circle); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_EXPONENTIAL>::apply(Circle); 
