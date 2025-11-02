#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/operations.hpp"

template <ImpulseType IT, UnitType UT, Function F>
AngDir2 Impulse<IT, UT, F>::apply(Square sq) {
  Dir2 diff = sq.get_position() - this->position;
  Dir2 diffa = diff.abs();
  diffa.x = max0 (diffa.x - sq.get_width());
  diffa.y = max0 (diffa.y - sq.get_height());
  if (diffa.modulo2() < this->radio * this->radio) {
    return AngDir2 {
      diff.x - bound (diff.x, sq.get_width()),
      diff.y - bound (diff.y, sq.get_height()),
      0
    }.normalize() * this->generated_force_coef;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::apply(Square);
