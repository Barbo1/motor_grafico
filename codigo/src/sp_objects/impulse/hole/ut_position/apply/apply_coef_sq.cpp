#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/operations.hpp"

template <ImpulseType IT, Function F>
float Impulse<IT, UT_POSITION, F>::apply_coef (Square sq) {
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

    if constexpr (F == FT_QUADRATIC)
      return coef * coef;
    else if constexpr (F == FT_CUBIC) {
      return coef * coef * coef;
    } else if constexpr (F == FT_SINUSOIDAL)
      return std::sin(2.f * M_PI * coef / this->radio);
    else if constexpr (F == FT_EXPONENTIAL)
      return std::exp(coef);
    else
      return coef;
  } else return 0.f;
}

template float Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::apply_coef(Square); 
template float Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::apply_coef(Square); 
template float Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::apply_coef(Square); 
template float Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::apply_coef(Square); 
template float Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::apply_coef(Square); 
