#pragma once

#include "../primitives/vectors.hpp"

struct Line {
  MemDir2 v;
  MemDir2 p;

  Line () noexcept;
  Line (float slope, float deviation) noexcept;
  Line (Dir2 p1, Dir2 p2) noexcept;

  float get_slope () const;
  float get_deviation () const;
};
