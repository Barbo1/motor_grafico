#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
Impulse<IT, UT, F>::Impulse (Global* glb, AngDir2 position, float radio, float generated) {
  this->glb = glb;
  this->position = position;
  this->radio = radio;
  this->generated_force_coef = generated;
}

template Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::Impulse (Global*, AngDir2, float, float); 
