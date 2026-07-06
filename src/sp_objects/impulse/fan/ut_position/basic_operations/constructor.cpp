#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
Impulse<IT_FAN, UT_POSITION, F>::Impulse (
  Global* glb, 
  Dir2 position, 
  float width, 
  float height, 
  float gfc, 
  FanImpDir dir
) noexcept {
  this->glb = glb;
  this->position = position;
  this->dimension = AngDir2 {width, height, 0};
  this->generated_force_coef = gfc;
  switch (dir) {
    case FanImpDir::FID_UP:
      this->direction = Dir2(0.f, 1.f);
      this->sign = 1;
      break;
    case FanImpDir::FID_DOWN:
      this->direction = Dir2(0.f, 1.f);
      this->sign = -1;
      break;
    case FanImpDir::FID_RIGHT:
      this->direction = Dir2(1.f, 0.f);
      this->sign = 1;
      break;
    case FanImpDir::FID_LEFT:
      this->direction = Dir2(1.f, 0.f);
      this->sign = -1;
      break;
  }
}

template Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::Impulse (Global*, Dir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::Impulse (Global*, Dir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::Impulse (Global*, Dir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::Impulse (Global*, Dir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::Impulse (Global*, Dir2, float, float, float, FanImpDir); 
