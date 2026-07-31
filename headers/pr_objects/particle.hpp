#pragma once

#include "../primitives/vectors.hpp"

#include <cstdint>

struct Particle {
  MemDir2 position;
  MemDir2 velocity;
  MemDir2 force;

  float mass;
  float radio;

  Particle () noexcept;
  Particle (Dir2 center, uint32_t radio, float mass) noexcept;
};
