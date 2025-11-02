#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
AngDir2 Impulse<IT, UT_POSITION, F>::get_position() {
  return this->position;
}

template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::get_position();
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::get_position();
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::get_position();
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::get_position();
template AngDir2 Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::get_position();
