#include "../../../headers/primitives/math.hpp"
#include <immintrin.h>
#include <cstdio>

void rotate_segments (std::pair<Dir2, Dir2>* segments, uint32_t many, float angle) {
  Dir2 cos_sin = cos_sin_c(angle);
  __m128 mat_rotation = _mm_xor_ps(
    _mm_set_ps(0.f, 0.f, -0.f, 0.f),
    _mm_shuffle_ps(cos_sin.v, cos_sin.v, 0b00010100)
  );

  for (uint32_t i = 0; i < many; i++) {
    std::pair<Dir2, Dir2>& segment = segments[i];

    __m128 first = _mm_mul_ps(mat_rotation, segment.first.v);
    __m128 second = _mm_mul_ps(mat_rotation, segment.second.v);
    segment.first.v = _mm_hadd_ps(first, first);
    segment.second.v = _mm_hadd_ps(second, second);
  }
}
