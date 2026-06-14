#include "./vectors.hpp"
#include <cstdint>
#include <utility>
#include <cmath>

void rotate_segments (std::pair<Dir2, Dir2>*, uint32_t, float);
void rotate_triangles (std::array<Dir2, 3>*, uint32_t, float);

inline bool test_collition_triangle_circle (Dir2 A, Dir2 vB, Dir2 vC, Dir2 cpos, float crad) {
  const Dir2 vCB = vC - vB;
  const Dir2 vDA = cpos - A;
  const Dir2 vDB = cpos - (vB + A);
  const Dir3 aux = Dir3 (
    (vB * vDA) / vB.modulo2(), 
    (vC * vDA) / vC.modulo2(), 
    (vCB * vDB) / vCB.modulo2()
  ).bound01();

  __m128 op1 = _mm_set_ss (vB.msub(aux.x, vDA).modulo2());
  __m128 op2 = _mm_set_ss (vC.msub(aux.y, vDA).modulo2());
  __m128 op3 = _mm_set_ss (vCB.msub(aux.z, vDB).modulo2());
  float minim = _mm_cvtss_f32 (_mm_min_ss (_mm_min_ss (op1, op2), op3));

  float c1 = vC.pLd(vDA, vB);
  float c2 = vB.pLd(vDA, vC);

  return minim < crad * crad || (0.f < c1 && 0.f < c2 && c1 + c2 < 1.f);
}

inline bool test_collition_triangle_segment (Dir2 A, Dir2 vB, Dir2 vC, Dir2 D, Dir2 vE) {
  float q1 = vB.pLd (vE, vC);
  float q2 = vC.pLd (vE, vB);
  Dir2 vAD = A - D;

  float v1 = vB.pLd(vAD, vE);
  float v2 = vC.pLd(vAD, vE);
  float v3 = (vC - vB).pLd(vB + vAD, vE);

  __m128 cond = _mm_set_ps (0.f, -(q1+q2), q2, q1);
  __m128 vals = _mm_set_ps (0.f, v3, v2, v1);
  __m128 aux = _mm_cmpgt_ps (cond, _mm_setzero_ps());
  __m128 cmax = _mm_and_ps (aux, vals);
  __m128 cmin = _mm_or_ps (_mm_andnot_ps(aux, vals), _mm_and_ps (aux, _mm_set1_ps(1.f)));
  __m128 cI_aux = _mm_max_ps (cmax, _mm_shuffle_ps (cmax, cmax, 0b10101010));
  __m128 cS_aux = _mm_min_ps (cmin, _mm_shuffle_ps (cmin, cmin, 0b10101010));
  __m128 cI = _mm_max_ss (cI_aux, _mm_shuffle_ps (cI_aux, cI_aux, 1));
  __m128 cS = _mm_min_ss (cS_aux, _mm_shuffle_ps (cS_aux, cS_aux, 1));

  return _mm_movemask_ps(_mm_cmpgt_ps(cS, cI)) & 1;
}

inline bool test_collition_triangle_point (Dir2 A, Dir2 vB, Dir2 vC, Dir2 P) {
  Dir2 PA = P - A;
  float c1 = vC.pLd(PA, vB);
  float c2 = vB.pLd(PA, vC);
  return (c1 > 0.f) & (c2 > 0.f) & (c1+c2 < 1.f);
}

inline bool test_collition_square_segment (Dir2 A, Dir2 dims, Dir2 E, Dir2 vD) {
  Dir2 vAE = A - E;

  __m128 dim_ext = _mm_shuffle_ps (dims.v, dims.v, 0b01010000);
  __m128 vd_ext = _mm_rcp_ps(_mm_shuffle_ps (vD.v, vD.v, 0b01010000));
  __m128 vae_ext = _mm_shuffle_ps (vAE.v, vAE.v, 0b01010000);
  __m128 res1 = _mm_mul_ps (dim_ext, vd_ext);
  __m128 vN_arr = _mm_fmaddsub_ps (vae_ext, vd_ext, res1); 
  
  __m128 cond = _mm_cmplt_ps (vN_arr, _mm_shuffle_ps (vN_arr, vN_arr, 0b10110001));
  __m128 cmin = _mm_and_ps (cond, vN_arr);
  __m128 cmax = _mm_or_ps(_mm_andnot_ps (cond, vN_arr), _mm_and_ps (cond, _mm_set1_ps(1.f)));
  __m128 cI_1 = _mm_max_ps(cmin, _mm_shuffle_ps (cmin, cmin, 0b10110001));
  __m128 cS_1 = _mm_min_ps(cmax, _mm_shuffle_ps (cmax, cmax, 0b10110001));
  __m128 cI = _mm_max_ss(cI_1, _mm_shuffle_ps (cI_1, cI_1, 0b00000010));
  __m128 cS = _mm_min_ss(cS_1, _mm_shuffle_ps (cS_1, cS_1, 0b00000010));

  return _mm_movemask_ps(_mm_cmpgt_ps(cS, cI)) & 1;
}

inline bool test_point_inside_square(const Dir2& P, const Dir2& SQP, const Dir2& SQD) {
  __m128 opr = _mm_mul_ps(_mm_sub_ps(SQP.v, P.v), _mm_rcp_ps(SQD.v));
  __m128i abs = _mm_and_si128(_mm_set1_epi32(0x7FFFFFFF), _mm_castps_si128(opr));
  __m128i cmp = _mm_cmpgt_epi32(_mm_set1_epi32(0x3F800000), abs);
  return (_mm_movemask_epi8(cmp) & 0xFF) == 0xFF;
}
