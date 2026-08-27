#pragma once

#include "./vectors.hpp"
#include <vector>

// enums

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

enum AntiAliasingType {
  AAx1,
  AAx2,
  AAx4,
  AAx8,
  AAx16
};

enum PhysicalConfigOptions {
  PCO_MOVIBLE = 0b1,
  PCO_IS_NORMAL = 0b10
};

enum PRObjType {
  PR_NONE = 0b0,
  PR_CIRCLE = 0b1,
  PR_SQUARE = 0b10,
  PR_LINE = 0b11,
  PR_NEDGE = 0b100,
  PR_PARTICLE = 0b101,
  PR_PROJECTILE = 0b110
};

enum ViewGeneration {
  VG_POINT,
  VG_DIRECTION
};

enum LightType {
  LT_DIRECTIONAL,
  LT_CENTERD,
  LT_FOCALIZED
};


// types

struct Light {
  std::array<Dir2, 2> focal_line;
  Dir2 position;
  struct LightColorTy {float r, g, b;} color;
  float intensity;
  float attenuation;
  LightType type;
};

struct MaskObject {
  MaskObject* next;
  MemDir2 point1;
  MemDir2 point2;
};

struct MaskObjectList {
  MaskObject* obj;
  std::size_t size;
};

struct SecondLevelElement {
  Dir2 point1;
  Dir2 point2;
  int32_t partition_offset;
};

struct FirstLevelElement {
  SecondLevelElement init_segment;
  std::vector<SecondLevelElement> data;
  int32_t first_level_offset;
  int32_t first_second_level_offset;
  int32_t last_second_level_offset;
};
