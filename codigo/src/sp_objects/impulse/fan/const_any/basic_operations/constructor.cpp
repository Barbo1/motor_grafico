#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
Impulse<IT_FAN, UT, F>::Impulse (
  Global* glb, 
  AngDir2 position, 
  AngDir2 force, 
  float width, 
  float height
) noexcept {
  this->glb = glb;
  this->force = force;
  this->position = position;
  this->dimension = AngDir2 {width, height, 0};
}

template Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::Impulse (Global*, AngDir2, AngDir2, float, float); 
