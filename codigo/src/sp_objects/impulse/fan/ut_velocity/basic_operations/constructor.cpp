#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
Impulse<IT_FAN, UT_VELOCITY, F>::Impulse (Global* glb, AngDir2 position, float width, float height, float gfc) {
  this->glb = glb;
  this->position = position;
  this->dimension = AngDir2 {width, height, 0};
  this->generated_force_coef = gfc;
}

template Impulse<IT_FAN, UT_VELOCITY, FT_LINEAR>::Impulse (Global*, AngDir2, float, float, float); 
template Impulse<IT_FAN, UT_VELOCITY, FT_QUADRATIC>::Impulse (Global*, AngDir2, float, float, float); 
template Impulse<IT_FAN, UT_VELOCITY, FT_CUBIC>::Impulse (Global*, AngDir2, float, float, float); 
template Impulse<IT_FAN, UT_VELOCITY, FT_SINUSOIDAL>::Impulse (Global*, AngDir2, float, float, float); 
template Impulse<IT_FAN, UT_VELOCITY, FT_EXPONENTIAL>::Impulse (Global*, AngDir2, float, float, float); 
