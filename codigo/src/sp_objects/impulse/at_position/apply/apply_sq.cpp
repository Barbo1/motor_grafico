#include "../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../headers/primitives/operations.hpp"

template <ImpulseType IT, Function F>
AngDir2 Impulse<IT, AT_POSITION, F>::apply(Square sq) {
  Dir2 diff = sq.get_position() - this->position;
  Dir2 diffa = diff.abs();
  diffa.x = max0 (diffa.x - sq.get_width());
  diffa.y = max0 (diffa.y - sq.get_height());
  if (diffa.modulo2() < this->radio * this->radio) {
    AngDir2 n = AngDir2 {
      bound (diff.x, sq.get_width()) - diff.x,
      bound (diff.y, sq.get_height()) - diff.y,
      0
    };

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

template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_LINEAR>::apply(Square); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_QUADRATIC>::apply(Square); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_CUBIC>::apply(Square); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_SINUSOIDAL>::apply(Square); 
template AngDir2 Impulse<IT_HOLE, AT_POSITION, FT_EXPONENTIAL>::apply(Square); 
