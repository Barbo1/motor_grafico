#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
Impulse<IT_FAN, UT_POSITION, F>::Impulse (
  Global* glb, 
  AngDir2 position, 
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
      this->direction = AngDir2 {0, 1, 0};
      this->sign = 1;
      break;
    case FanImpDir::FID_DOWN:
      this->direction = AngDir2 {0, 1, 0};
      this->sign = -1;
      break;
    case FanImpDir::FID_RIGHT:
      this->direction = AngDir2 {1, 0, 0};
      this->sign = 1;
      break;
    case FanImpDir::FID_LEFT:
      this->direction = AngDir2 {1, 0, 0};
      this->sign = -1;
      break;
  }
}

template Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::Impulse (Global*, AngDir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::Impulse (Global*, AngDir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::Impulse (Global*, AngDir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::Impulse (Global*, AngDir2, float, float, float, FanImpDir); 
template Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::Impulse (Global*, AngDir2, float, float, float, FanImpDir); 
