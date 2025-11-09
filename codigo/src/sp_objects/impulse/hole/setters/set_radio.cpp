#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
void Impulse<IT, UT, F>::set_radio(float r) {
  this->radio = r;
}

template void Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::set_radio(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::set_radio(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::set_radio(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::set_radio(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::set_radio(float);
template void Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::set_radio(float);
