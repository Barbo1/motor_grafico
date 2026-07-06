#include "../../../../../../headers/sp_objects/impulse.hpp"
#include "../../../../../../headers/primitives/math.hpp"

template <UnitType UT, Function F>
AngDir2 Impulse<IT_FAN, UT, F>::apply(Square& sq) {
  Dir2 diff = (this->position - sq.get_position()).abs();
  Dir2 size = Dir2 (sq.get_width(), sq.get_height()) + this->dimension;
  if (diff < size) {
    return this->force;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::apply(Square&); 
