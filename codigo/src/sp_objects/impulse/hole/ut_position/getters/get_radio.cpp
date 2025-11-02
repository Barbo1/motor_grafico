#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
float Impulse<IT, UT_POSITION, F>::get_radio() {
  return this->radio;
}

template float Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::get_radio();
template float Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::get_radio();
template float Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::get_radio();
template float Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::get_radio();
template float Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::get_radio();
