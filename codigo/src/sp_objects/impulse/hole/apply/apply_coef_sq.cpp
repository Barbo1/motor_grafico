#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
float Impulse<IT, UT, F>::apply_coef (Square& sq) {
  AngDir2 diff = sq.get_position() - this->position;
  AngDir2 size = AngDir2 {sq.get_width(), sq.get_height(), 0.f};
  AngDir2 diffa = (diff.abs() - size).max0();
  if (diffa.modulo2() < this->radio * this->radio) {
    AngDir2 n = diff - diff.bound(size);
    float coef = n.modulo() * this->generated_force_coef;

    if constexpr (F == FT_LINEAR)
      return coef;
    else if constexpr (F == FT_QUADRATIC)
      return coef * coef;
    else if constexpr (F == FT_CUBIC)
      return coef * coef * coef;
    else if constexpr (F == FT_SINUSOIDAL)
      return std::sin(2.f * M_PI * coef / this->radio);
    else if constexpr (F == FT_EXPONENTIAL)
      return std::exp(coef);
    else
      return this->generated_force_coef;
  } else return 0.f;
}

template float Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::apply_coef(Square&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::apply_coef(Square&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::apply_coef(Square&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::apply_coef(Square&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::apply_coef(Square&); 
template float Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::apply_coef(Square&); 
