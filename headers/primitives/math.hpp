#include "./vectors.hpp"
#include <cstdint>
#include <immintrin.h>
#include <pmmintrin.h>
#include <utility>
#include <cmath>

void rotate_segments (std::pair<Dir2, Dir2>*, uint32_t, float);
void rotate_triangles (std::array<Dir2, 3>*, uint32_t, float);

/* Test if there is a collision between a triangle and a point.
 *
 * 'P' is a point,
 * 'A' is a vertex of the triangle,
 * 'vB' is a vector that goes from 'A' to another vertex,
 * 'vC' is a vector that goes from 'A' to another vertex, 
 *    different that the one pointed by 'vB'.
 * */
inline bool test_collision_triangle_point (Dir2 A, Dir2 vB, Dir2 vC, Dir2 P) {
  __m128 PA = _mm_sub_ps(P.v, A.v);
  __m128 PAL_ext = _mm_shuffle_ps(PA, PA, 0b00010001);
  __m128 BC_ext = _mm_movelh_ps(vB.v, vC.v);
  __m128 vCB = _mm_sub_ps(vC.v, vB.v);
  
  __m128 num_mult_1 = _mm_mul_ps(BC_ext, PAL_ext);
  __m128 num_mult_2 = _mm_mul_ps(vCB, PAL_ext);
  
  __m128 den_mult_1 = _mm_shuffle_ps(vB.v, vB.v, 0b00010001);
  __m128 den_mult_2 = _mm_movelh_ps(vC.v, vC.v);

  __m128 num = _mm_hsub_ps(num_mult_1,  num_mult_2);
  __m128 den = _mm_mul_ps(den_mult_1, den_mult_2);
  den = _mm_hsub_ps(den, den);

  __m128 cond_1 = _mm_xor_ps(num, den);
  __m128 cond_2 = _mm_xor_ps(_mm_cmple_ps(num, den), den);
  
  return _mm_movemask_ps(_mm_shuffle_ps(cond_1, cond_2, 0b10100100)) == 0b1101;
}

/* Given a point, and the position and dimentions of a square, return true
 * if the point is inside of the square.
 *
 * 'P' is the point,
 * 'SQP' is the center of the square,
 * 'SQD' are the dimentions of the square.
 * */
inline bool test_point_inside_square(const Dir2& P, const Dir2& SQP, const Dir2& SQD) {
  __m128 opr = _mm_mul_ps(_mm_sub_ps(SQP.v, P.v), _mm_rcp_ps(SQD.v));
  __m128i abs = _mm_and_si128(_mm_set1_epi32(0x7FFFFFFF), _mm_castps_si128(opr));
  __m128i cmp = _mm_cmpgt_epi32(_mm_set1_epi32(0x3F800000), abs);
  return (_mm_movemask_epi8(cmp) & 0xFF) == 0xFF;
}

/* Test if there is a collision between a triangle and a circle.
 *
 * 'cpos' is the position of the circle,
 * 'crad' is the radio of the circle,
 * 'A' is a vertex of the triangle,
 * 'vB' is a vector that goes from 'A' to another vertex,
 * 'vC' is a vector that goes from 'A' to another vertex, 
 *    different that the one pointed by 'vB'.
 * */
inline bool test_collision_triangle_circle (Dir2 A, Dir2 vB, Dir2 vC, Dir2 cpos, float crad) {
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

/* Test if there is a collision between a triangle and a point.
 *
 * 'P' is a point,
 * 'A' is a vertex of the triangle,
 * 'vB' is a vector that goes from 'A' to another vertex,
 * 'vC' is a vector that goes from 'A' to another vertex, 
 *    different that the one pointed by 'vB'.
 * */
inline bool test_collision_triangle_segment (Dir2 A, Dir2 vB, Dir2 vC, Dir2 D, Dir2 vE) {
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

/* Test if there is a collision between a square and a segment.
 *
 * 'A' is the center of the square,
 * 'dims' are the dimenions of the square,
 * 'E' is the begining point of the segment,
 * 'vD' is the direction of the segment(finishing in the last point of the segment).
 * */
inline bool test_collision_square_segment (Dir2 A, Dir2 dims, Dir2 E, Dir2 vD) {
  __m128 vAE = _mm_sub_ps(A.v, E.v);

  __m128 dim_ext = _mm_xor_ps(
    _mm_shuffle_ps (dims.v, dims.v, 0b01010000), 
    _mm_set_ps(0.f, -0.f, 0.f, -0.f)
  );
  __m128 vd_ext = _mm_rcp_ps(_mm_shuffle_ps (vD.v, vD.v, 0b01010000));
  __m128 vae_ext = _mm_shuffle_ps (vAE, vAE, 0b01010000);
  __m128 res1 = _mm_add_ps (vae_ext, dim_ext);
  __m128 vN_arr = _mm_mul_ps (res1, vd_ext); 
  
  __m128 cond = _mm_cmplt_ps (vN_arr, _mm_shuffle_ps (vN_arr, vN_arr, 0b10110001));
  __m128 cmin = _mm_and_ps (cond, vN_arr);
  __m128 cmax = _mm_or_ps(_mm_andnot_ps (cond, vN_arr), _mm_and_ps (cond, _mm_set1_ps(1.f)));
  __m128 cI_1 = _mm_max_ps(cmin, _mm_shuffle_ps (cmin, cmin, 0b10110001));
  __m128 cS_1 = _mm_min_ps(cmax, _mm_shuffle_ps (cmax, cmax, 0b10110001));
  __m128 cI = _mm_max_ss(cI_1, _mm_shuffle_ps (cI_1, cI_1, 0b00000010));
  __m128 cS = _mm_min_ss(cS_1, _mm_shuffle_ps (cS_1, cS_1, 0b00000010));

  return _mm_movemask_ps(_mm_cmpgt_ps(cS, cI)) & 1;
}

/* Test if there is a collision between two triangles.
 *
 * 'A' is the center of the first triangle,
 * 'vB' and 'vC' are the vectors from 'A' to the other two verteces,
 * 'D' is the center of the second triangle,
 * 'vE' and 'vF' are the vectors from 'D' to the other two verteces,
 * */
inline bool test_collision_triangle_triangle(Dir2 A, Dir2 vB, Dir2 vC, Dir2 D, Dir2 vE, Dir2 vF) {
  __m128 D_ext = _mm_movelh_ps(D.v, D.v);
  __m128 A_ext = _mm_movelh_ps(A.v, A.v);
  __m128 AD_ext = _mm_sub_ps(A_ext, D_ext);
  __m128 vEL_ext = _mm_shuffle_ps(vE.v, vE.v, 0b00010001);
  __m128 vFL_ext = _mm_shuffle_ps(vF.v, vF.v, 0b00010001);
  __m128 vBL_ext = _mm_shuffle_ps(vB.v, vB.v, 0b00010001);
  __m128 vCL_ext = _mm_shuffle_ps(vC.v, vC.v, 0b00010001);

  __m256 AD_ext_256 = _mm256_setr_m128(AD_ext, AD_ext);
  __m256 vBL_ext_256 = _mm256_setr_m128(vBL_ext, vBL_ext);
  __m256 vCL_ext_256 = _mm256_setr_m128(vCL_ext, vCL_ext);

  // calculating vector intersection.
  __m128 FE_ext = _mm_movelh_ps(vF.v, vE.v);
  __m128 FEL_ext = _mm_shuffle_ps(FE_ext, FE_ext, 0b10110001);
  __m256 FE_ext_256 = _mm256_setr_m128(FE_ext, FE_ext);

  __m256 numer_inter = _mm256_hsub_ps(
    _mm256_mul_ps(_mm256_setr_m128(vBL_ext, FEL_ext), AD_ext_256), 
    _mm256_mul_ps(_mm256_setr_m128(vCL_ext, FEL_ext), AD_ext_256)
  );
  __m256 denom_inter = _mm256_hsub_ps(
    _mm256_mul_ps(vBL_ext_256, FE_ext_256), 
    _mm256_mul_ps(vCL_ext_256, FE_ext_256)
  );

  __m256 cond_inter_1 = _mm256_xor_ps(numer_inter, denom_inter);
  __m256 cond_inter_2 = _mm256_cmp_ps(numer_inter, denom_inter, 2);
  __m256 cond_inter_f = _mm256_andnot_ps(
    cond_inter_1, _mm256_xor_ps(cond_inter_2, denom_inter)
  );

  // calculating point fitting.
  __m128 denom_aux_1 = _mm_mul_ps(vFL_ext, _mm_movelh_ps(vE.v, vE.v));
  __m128 denom_aux_2 = _mm_mul_ps(vCL_ext, _mm_movelh_ps(vB.v, vB.v));
  __m256 denom_aux_3 = _mm256_setr_m128(denom_aux_1, denom_aux_1);
  __m256 denom_aux_4 = _mm256_setr_m128(denom_aux_2, denom_aux_2);
  __m256 denom = _mm256_hsub_ps(denom_aux_3, denom_aux_4);

  __m256 diffs_1 = _mm256_add_ps(
    _mm256_setr_m128(_mm_setzero_ps(), _mm_movelh_ps(vC.v, vB.v)),
    AD_ext_256
  );
  __m256 diffs_2 = _mm256_sub_ps(
    _mm256_setr_m128(_mm_setzero_ps(), _mm_movelh_ps(vF.v, vE.v)),
    AD_ext_256
  );

  // calculating parts.
  __m256 x_part_1 = _mm256_mul_ps(diffs_1, _mm256_setr_m128(vFL_ext, vFL_ext));
  __m256 x_part_2 = _mm256_mul_ps(diffs_2, vCL_ext_256);
  __m256 x_part = _mm256_hsub_ps(x_part_1, x_part_2);
  __m256 y_part_1 = _mm256_mul_ps(diffs_1, _mm256_setr_m128(vEL_ext, vEL_ext));
  __m256 y_part_2 = _mm256_mul_ps(diffs_2, vBL_ext_256);
  __m256 y_part = _mm256_hsub_ps(y_part_1, y_part_2);

  // calculating condition.
  __m256 cond_fit_1 = _mm256_xor_ps(x_part, denom);
  __m256 cond_fit_2 = _mm256_xor_ps(y_part, denom);
  __m256 cond_fit_3 = _mm256_cmp_ps(_mm256_sub_ps(x_part, y_part), denom, 2);
  __m256 cond_fit_f = _mm256_andnot_ps(
    cond_fit_1, 
    _mm256_and_ps(cond_fit_2, _mm256_xor_ps(cond_fit_3, denom))
  );

  // returning.
  int c1 = _mm256_movemask_ps(cond_inter_f);
  int c2 = _mm256_movemask_ps(cond_fit_f);
  return (c1 & (c1 >> 4)) | c2;
}

/* Return the direction from the square to the segment, and if they don't collide,
 * return Dir2(0.f, 0.f).
 *
 * 'A' is the center of the square,
 * 'dims' are the dimenions of the square,
 * 'E' is the begining point of the segment,
 * 'vD' is the direction of the segment(finishing in the last point of the segment).
 * */
inline Dir2 collision_direction_square_segment (Dir2 A, Dir2 dims, Dir2 E, Dir2 vD) {
  __m128 vAE = _mm_sub_ps(A.v, E.v);
  __m128 vae_ext = _mm_shuffle_ps (vAE, vAE, 0b01010000);
  __m128 dim_ext = _mm_xor_ps(
    _mm_shuffle_ps (dims.v, dims.v, 0b01010000), 
    _mm_set_ps(-0.f, 0.f, -0.f, 0.f)
  );
  __m128 vd_ext = _mm_rcp_ps(_mm_shuffle_ps (vD.v, vD.v, 0b01010000));
  __m128 res1 = _mm_add_ps (vae_ext, dim_ext);
  __m128 vN_arr = _mm_mul_ps (res1, vd_ext);

  __m128 part_1 = _mm_shuffle_ps (vN_arr, vN_arr, 0b00001000);
  __m128 part_2 = _mm_shuffle_ps (vN_arr, vN_arr, 0b00001101);

  __m128 cI_prev = _mm_min_ps(part_1, part_2);
  __m128 cS_prev = _mm_max_ps(part_1, part_2);
  __m128 cI = _mm_max_ps(cI_prev, _mm_shuffle_ps (cI_prev, cI_prev, 0b00000001));
  __m128 cS = _mm_min_ps(cS_prev, _mm_shuffle_ps (cS_prev, cS_prev, 0b00000001));

  __m128 c_sum = _mm_mul_ps(_mm_add_ps(cI, cS), _mm_set1_ps(0.5f));

  __m128 test_cond_fin = _mm_cmplt_ps(
    _mm_max_ps(cI, _mm_setzero_ps()),
    _mm_min_ps(cS, _mm_set1_ps(1.f))
  );

  __m128 c_sum_fin = _mm_max_ps (
    _mm_setzero_ps(), 
    _mm_min_ps(
      _mm_set1_ps(1.f), 
      c_sum
    )
  );

  return Dir2(_mm_and_ps(
    test_cond_fin, 
    _mm_fmsub_ps(vD.v, c_sum_fin, vAE)
  ));
}

/* This function recieves a square information, the segments known to 
 * collide with it, and the direction of reposition, and calculate the
 * distance(multiplicative distance) for the reposition through it.
 *
 * 'C' is the center of the square,
 * 'dims' are the dimentions of the square,
 * 'd' is the direction of the reposition,
 * 'segments' are store segments,
 * 'many' is the number of segments avaiable in 'segments'.
 * */
inline Dir2 directional_distance_square_segment (Dir2 C, Dir2 dims, Dir2 d, std::pair<Dir2, Dir2>* segments, uint32_t many) {
  __m128 half = _mm_set1_ps(0.5f);
  __m128 dim_sgn = _mm_xor_ps(dims.v, _mm_and_ps(_mm_set1_ps(-0.f), d.v));
  __m128 dim_sgn_inverse = _mm_xor_ps(_mm_set_ps(-0.f, 0.f, -0.f, 0.f), dims.v);

  __m128 D = _mm_add_ps(C.v, dim_sgn);
  __m128 D_I = _mm_movelh_ps(D, _mm_sub_ps(C.v, dim_sgn));
  __m128 P1_P2 = _mm_movelh_ps(_mm_add_ps(C.v, dim_sgn_inverse), _mm_sub_ps(C.v, dim_sgn_inverse));

  __m128 d_ext = _mm_shuffle_ps(d.v, d.v, 0b00010001);
  __m128 curr_min = _mm_setzero_ps();
  for (uint32_t i = 0; i < many; i++) {
    Dir2 A = segments[i].first;
    Dir2 v = segments[i].second;

    __m128 B = _mm_add_ps(A.v, v.v);
    __m128 A2 = _mm_movelh_ps(A.v, A.v);

    // part 1.
    {
      __m128 v_ext = _mm_shuffle_ps(v.v, v.v, 0b00010001);
      __m128 M_mid = _mm_mul_ps(_mm_add_ps(A.v, B), half);
      __m128 M = _mm_movelh_ps(M_mid, M_mid);
      __m128 denom = _mm_rcp_ps(
        _mm_set1_ps(
          _mm_cvtss_f32(
            _mm_hsub_ps(_mm_mul_ps(d.v, v_ext), _mm_undefined_ps())
          )
        )
      );
     
      // calculating condition 1.
      __m128 cond_1_middle_1 = _mm_mul_ps(_mm_sub_ps(D_I, M), d_ext);
      __m128 cond_1_middle_2 = _mm_mul_ps(_mm_sub_ps(P1_P2, M), d_ext);
      __m128 cond_1_middle = _mm_hsub_ps(cond_1_middle_1, cond_1_middle_2);
      __m128 cond_1 = _mm_cmplt_ps(
        _mm_and_ps(
          _mm_mul_ps(cond_1_middle, denom),
          _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))
        ),
        half
      );
      
      // calculating value 1.
      __m128 value_1_middle_1 = _mm_mul_ps(_mm_sub_ps(A2, D_I), v_ext);
      __m128 value_1_middle_2 = _mm_mul_ps(_mm_sub_ps(A2, P1_P2), v_ext);
      __m128 value_1_middle = _mm_hsub_ps(value_1_middle_1, value_1_middle_2);
      curr_min = _mm_min_ps(
        _mm_and_ps(cond_1, _mm_mul_ps(value_1_middle, denom)), 
        curr_min
      );
    }
   
    // part 2.
    {
      __m128 D2 = _mm_movelh_ps(D, D);
      __m128 B2 = _mm_movelh_ps(B, B);
      __m128 v12_ext = _mm_sub_ps(D2, P1_P2);
      __m128 M_12 = _mm_add_ps(D2, P1_P2);
      __m128 denom = _mm_mul_ps(v12_ext, d_ext);
      v12_ext = _mm_shuffle_ps(v12_ext, v12_ext, 0b10110001);
      denom = _mm_rcp_ps(_mm_hadd_ps(denom, denom));
     
      // calculating condition 2.
      __m128 cond_2_middle_2 = _mm_mul_ps(_mm_fmsub_ps(M_12, half, B2), d_ext);
      __m128 cond_2_middle = _mm_fmsub_ps(_mm_fmsub_ps(M_12, half, A2), d_ext, cond_2_middle_2);
      __m128 cond_2 = _mm_cmplt_ps(
        _mm_and_ps(
          _mm_mul_ps(cond_2_middle, denom),
          _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))
        ),
        half
      );
      
      // calculating value 2.
      __m128 value_2_middle_1 = _mm_mul_ps(_mm_sub_ps(P1_P2, A2), v12_ext);
      __m128 value_2_middle_2 = _mm_mul_ps(_mm_sub_ps(P1_P2, B2), v12_ext);
      __m128 value_2_middle = _mm_hsub_ps(value_2_middle_1, value_2_middle_2);
      curr_min = _mm_min_ps(
        _mm_and_ps(cond_2, _mm_mul_ps(value_2_middle, denom)), 
        curr_min
      );
    }
  }

  curr_min = _mm_min_ps(curr_min, _mm_shuffle_ps (curr_min, curr_min, 0b00001101));
  curr_min = _mm_min_ps(curr_min, _mm_shuffle_ps (curr_min, curr_min, 0b00000001));
  return d * _mm_cvtss_f32(curr_min);
}
