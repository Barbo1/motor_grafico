#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
AngDir2 Impulse<IT_FAN, UT, F>::apply(Square& sq) {
  AngDir2 diff = (this->position - sq.get_position()).abs();
  AngDir2 size = AngDir2 {sq.get_width(), sq.get_height(), 0} + this->dimension;
  if (diff.y < size.y && diff.x < size.x) {
    return this->force;
  } else return AngDir2();
}

template AngDir2 Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::apply(Square&); 
