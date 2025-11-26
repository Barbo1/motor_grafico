#pragma once

enum ParSrcType {
  PS_SMOOTH,
  PS_EXPLOSION
};

enum Function {
  FT_CONSTANT,
  FT_LINEAR,
  FT_LINEAR_N,
  FT_QUADRATIC,
  FT_QUADRATIC_N,
  FT_CUBIC,
  FT_CUBIC_N,
  FT_EXPONENTIAL,
  FT_EXPONENTIAL_N,
  FT_SINUSOIDAL,
  FT_SINUSOIDAL_N,
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
