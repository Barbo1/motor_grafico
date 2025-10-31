#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, Function F>
Impulse<IT, AT_POSITION, F>::Impulse (Global* glb, AngDir2 position, float radio, float generated) {
  this->glb = glb;
  this->position = position;
  this->radio = radio;
  this->generated_force_coef = generated;
}

template Impulse<IT_HOLE, AT_POSITION, FT_LINEAR>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, AT_POSITION, FT_QUADRATIC>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, AT_POSITION, FT_CUBIC>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, AT_POSITION, FT_SINUSOIDAL>::Impulse (Global*, AngDir2, float, float); 
template Impulse<IT_HOLE, AT_POSITION, FT_EXPONENTIAL>::Impulse (Global*, AngDir2, float, float); 
