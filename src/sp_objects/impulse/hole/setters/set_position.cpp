#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
void Impulse<IT, UT, F>::set_position(Dir2& position) {
  this->position = position;
}

template void Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::set_position(Dir2&);
template void Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::set_position(Dir2&);
template void Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::set_position(Dir2&);
template void Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::set_position(Dir2&);
template void Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::set_position(Dir2&);
template void Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::set_position(Dir2&);
