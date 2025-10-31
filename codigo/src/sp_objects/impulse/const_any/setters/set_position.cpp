#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, AngType AT, Function F>
void Impulse<IT, AT, F>::set_position(AngDir2 position) {
  this->position = position;
}

template void Impulse<IT_HOLE, AT_NONE, FT_CONSTANT>::set_position(AngDir2);
