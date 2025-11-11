#pragma once

enum Function {
  FT_CONSTANT,
  FT_LINEAR,
  FT_QUADRATIC,
  FT_CUBIC,
  FT_EXPONENTIAL,
  FT_SINUSOIDAL,
};

enum UnitType {
  UT_NONE,
  UT_TIME,
  UT_POSITION,
  UT_VELOCITY,
};

enum ImpulseType {
  IT_HOLE,
  IT_FAN
};

enum FanImpDir {
  FID_UP,
  FID_DOWN,
  FID_LEFT,
  FID_RIGHT
};
