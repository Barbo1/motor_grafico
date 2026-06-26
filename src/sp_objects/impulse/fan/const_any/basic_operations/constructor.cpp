#include "../../../../../../headers/sp_objects/impulse.hpp"

template <UnitType UT, Function F>
Impulse<IT_FAN, UT, F>::Impulse (
  Global* glb, 
  Dir2 position, 
  AngDir2 force, 
  float width, 
  float height
) noexcept {
  this->glb = glb;
  this->force = force;
  this->position = position;
  this->dimension = Dir2(width, height);
}

template Impulse<IT_FAN, UT_NONE, FT_CONSTANT>::Impulse (Global*, Dir2, AngDir2, float, float); 
