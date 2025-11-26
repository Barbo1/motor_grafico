#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
Impulse<IT, UT, F>::Impulse (Global* glb, AngDir2 position, float radio, float generated) noexcept {
  this->glb = glb;
  this->position = position;
  this->radio = radio;
  this->generated_force_coef = generated;
}

template Impulse<IT_HOLE, UT_POSITION, FT_CONSTANT>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, UT_POSITION, FT_LINEAR>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, UT_POSITION, FT_QUADRATIC>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, UT_POSITION, FT_CUBIC>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, UT_POSITION, FT_SINUSOIDAL>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, UT_POSITION, FT_EXPONENTIAL>::Impulse (Global*, AngDir2, float, float); 
