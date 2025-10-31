#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, AngType AT, Function F>
void Impulse<IT, AT, F>::set_radio(float r) {
  this->radio = r;
}

template void Impulse<IT_HOLE, AT_NONE, FT_CONSTANT>::set_radio(float);
