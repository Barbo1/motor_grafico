#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, AngType AT, Function F>
AngDir2 Impulse<IT, AT, F>::get_position() {
  return this->position;
}

template AngDir2 Impulse<IT_HOLE, AT_NONE, FT_CONSTANT>::get_position();
