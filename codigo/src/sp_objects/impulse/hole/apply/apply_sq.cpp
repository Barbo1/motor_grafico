#include "../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../headers/primitives/operations.hpp"

template <ImpulseType IT, UnitType UT, Function F>
AngDir2 Impulse<IT, UT, F>::apply(Square& sq) {
  Dir2 diff = sq.get_position() - this->position;
  Dir2 diffa = diff.abs();
  diffa.x = max0 (diffa.x - sq.get_width());
  diffa.y = max0 (diffa.y - sq.get_height());
  if (diffa.modulo2() < this->radio * this->radio) {
    AngDir2 n = AngDir2 {
      diff.x - bound (diff.x, sq.get_width()),
      diff.y - bound (diff.y, sq.get_height()),
      0
    };
    float coef = n.modulo() * this->generated_force_coef;

    if constexpr (F == FT_LINEAR)
      return n * this->generated_force_coef;
    else if constexpr (F == FT_QUADRATIC)
      return n * this->generated_force_coef * coef;
    else if constexpr (F == FT_CUBIC)
      return n * this->generated_force_coef * coef * coef;
    else if constexpr (F == FT_SINUSOIDAL)
      return n.normalize() * std::sin(2.f * M_PI * coef / this->radio);
    else if constexpr (F == FT_EXPONENTIAL)
      return n.normalize() * std::exp(coef);
    else
      return n.normalize() * this->generated_force_coef;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::apply(Square&); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::apply(Square&); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::apply(Square&); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::apply(Square&); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::apply(Square&); 
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::apply(Square&); 
