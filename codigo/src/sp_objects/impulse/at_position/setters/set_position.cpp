#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
void Impulse<IT, AT_POSITION, F>::set_position(AngDir2 position) {
  this->position = position;
}

template void Impulse<IT_HOLE, AT_POSITION, FT_LINEAR>::set_position(AngDir2);
template void Impulse<IT_HOLE, AT_POSITION, FT_QUADRATIC>::set_position(AngDir2);
template void Impulse<IT_HOLE, AT_POSITION, FT_CUBIC>::set_position(AngDir2);
template void Impulse<IT_HOLE, AT_POSITION, FT_SINUSOIDAL>::set_position(AngDir2);
template void Impulse<IT_HOLE, AT_POSITION, FT_EXPONENTIAL>::set_position(AngDir2);
