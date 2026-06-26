#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_POSITION, F>::set_position(Dir2& position) {
  this->position = position;
}

template void Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::set_position(Dir2&); 
template void Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::set_position(Dir2&);
template void Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::set_position(Dir2&);
template void Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::set_position(Dir2&);
template void Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::set_position(Dir2&);
