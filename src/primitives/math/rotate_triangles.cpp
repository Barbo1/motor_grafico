#include "../../../headers/primitives/math.hpp"
#include <array> 

void rotate_triangles (std::array<Dir2, 3>* triangles, uint32_t many, float angle) {
  Dir2 cos_sin = cos_sin_c(angle);
  __m128 mat_rotation = _mm_xor_ps(
    _mm_set_ps(0.f, 0.f, -0.f, 0.f),
    _mm_shuffle_ps(cos_sin.v, cos_sin.v, 0b00010100)
  );

  for (uint32_t i = 0; i < many; i++) {
    std::array<Dir2, 3>& triangle = triangles[i];
    Dir2& point = triangle[0]; 
    Dir2& vec1 = triangle[1]; 
    Dir2& vec2 = triangle[2]; 

    __m128 aux_p = _mm_mul_ps(mat_rotation, point.v);
    __m128 aux_1 = _mm_mul_ps(mat_rotation, vec1.v);
    __m128 aux_2 = _mm_mul_ps(mat_rotation, vec2.v);
    point.v = _mm_hadd_ps(aux_p, aux_p);
    vec1.v = _mm_hadd_ps(aux_1, aux_1);
    vec2.v = _mm_hadd_ps(aux_2, aux_2);
  }
}
