#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/operations.hpp"

template <ImpulseType IT, UnitType UT, Function F>
float Impulse<IT, UT, F>::apply_coef (Square sq) {
  Dir2 diffa = (sq.get_position() - this->position).abs();
  diffa.x = max0 (diffa.x - sq.get_width());
  diffa.y = max0 (diffa.y - sq.get_height());
  if (diffa.modulo2() < this->radio * this->radio) {
    return this->generated_force_coef;
  } else return 0.f;
}

template float Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::apply_coef (Square);
