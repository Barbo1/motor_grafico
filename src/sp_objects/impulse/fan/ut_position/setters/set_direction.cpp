#include "../../../../../../headers/sp_objects/impulse.hpp"

template <Function F>
void Impulse<IT_FAN, UT_POSITION, F>::set_direction(FanImpDir dir) {
  switch (dir) {
    case FanImpDir::FID_UP:
    case FanImpDir::FID_DOWN:
      this->direction = AngDir2 {0, 1, 0};
      break;
    case FanImpDir::FID_RIGHT:
    case FanImpDir::FID_LEFT:
      this->direction = AngDir2 {1, 0, 0};
      break;
  }
  switch (dir) {
    case FanImpDir::FID_RIGHT:
    case FanImpDir::FID_UP:
      this->sign = 1;
      break;
    case FanImpDir::FID_DOWN:
    case FanImpDir::FID_LEFT:
      this->sign = -1;
      break;
  }
}

template void Impulse<IT_FAN, UT_POSITION, FT_LINEAR>::set_direction(FanImpDir); 
template void Impulse<IT_FAN, UT_POSITION, FT_QUADRATIC>::set_direction(FanImpDir);
template void Impulse<IT_FAN, UT_POSITION, FT_CUBIC>::set_direction(FanImpDir);
template void Impulse<IT_FAN, UT_POSITION, FT_SINUSOIDAL>::set_direction(FanImpDir);
template void Impulse<IT_FAN, UT_POSITION, FT_EXPONENTIAL>::set_direction(FanImpDir);
