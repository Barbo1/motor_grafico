#pragma once

#include "./physical.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/nedge.hpp"
#include "../pr_objects/line.hpp"
#include "../pr_objects/particle.hpp"
#include <cstdint>


/* Take two objects and verifies if they have collide. The 
 * order of the parameters are from least to most complex. 
 * */
bool test_collition (const Line&, Physical&);
bool test_collition (const Line&, const Particle&);
bool test_collition (const Line&, const Circle&);
bool test_collition (const Line&, const Square&);
template<std::size_t N> bool test_collition (const Line&, const NEdge<N>&);

bool test_collition (const Particle&, Physical&);
bool test_collition (const Particle&, const Circle&);
bool test_collition (const Particle&, const Square&);
template<std::size_t N> bool test_collition (const Particle&, const NEdge<N>&);

bool test_collition (Physical&, Physical&);
bool test_collition (const Circle&, const Circle&);
bool test_collition (const Circle&, const Square&);
bool test_collition (const Square&, const Square&);
template<std::size_t N> bool test_collition (const Circle&, const NEdge<N>&);
template<std::size_t N> bool test_collition (const Square&, const NEdge<N>&);
template<std::size_t N, std::size_t M> bool test_collition (const NEdge<N>&, const NEdge<M>&);



/* Take two objects and generate the collition. The order 
 * of the parameters are from least to most complex. 
 * */
void resolve_collition (Physical&, Line&);
void resolve_collition (Particle&, Line&);
void resolve_collition (Circle&, Line&);
void resolve_collition (Square&, Line&);
template<std::size_t N> void resolve_collition (NEdge<N>&, Line&);

void resolve_collition (Particle&, Physical&);
void resolve_collition (Particle&, Square&);
void resolve_collition (Particle&, Circle&);
template<std::size_t N> void resolve_collition (Particle&, NEdge<N>&);

void resolve_collition (Physical&, Physical&);
void resolve_collition (Circle&, Circle&);
void resolve_collition (Circle&, Square&);
void resolve_collition (Square&, Circle&);
void resolve_collition (Square&, Square&);
template<std::size_t N> void resolve_collition (Circle&, NEdge<N>&);
template<std::size_t N> void resolve_collition (Square&, NEdge<N>&);
template<std::size_t N> void resolve_collition (NEdge<N>&, Circle&);
template<std::size_t N> void resolve_collition (NEdge<N>&, Square&);
template<std::size_t N, std::size_t M> void resolve_collition (NEdge<N>&, NEdge<M>&);



/* Given that a collition had ocurred, this function corrects 
 * the superposition of the objects moving the first slightly, 
 * so that the only point in it is the one returned by 
 * collition_point.
 * */

void correct_collition (Physical&, Line&);
void correct_collition (Particle&, Line&);
void correct_collition (Square&, Line&);
void correct_collition (Circle&, Line&);
template<std::size_t N> void correct_collition (NEdge<N>&, Line&);

void correct_collition (Particle&, Physical&);
void correct_collition (Particle&, Square&);
void correct_collition (Particle&, Circle&);
template<std::size_t N> void correct_collition (Particle&, NEdge<N>&);

void correct_collition (Physical&, Physical&);
void correct_collition (Circle&, Circle&);
void correct_collition (Circle&, Square&);
void correct_collition (Square&, Circle&);
void correct_collition (Square&, Square&);
template<std::size_t N> void correct_collition (Circle&, NEdge<N>&);
template<std::size_t N> void correct_collition (Square&, NEdge<N>&);
template<std::size_t N> void correct_collition (NEdge<N>&, Circle&);
template<std::size_t N> void correct_collition (NEdge<N>&, Square&);
template<std::size_t N, std::size_t M> void correct_collition (NEdge<N>&, NEdge<M>&);



/* Returns the point of the collition between two objects. */
Dir2 collition_point (const Line&, const Physical&);
Dir2 collition_point (const Line&, const Square&);
Dir2 collition_point (const Line&, const Circle&);
template<std::size_t N> Dir2 collition_point (const Line&, const NEdge<N>&);

Dir2 collition_point (const Physical&, const Physical&);
Dir2 collition_point (const Circle&, const Circle&);
Dir2 collition_point (const Circle&, const Square&);
Dir2 collition_point (const Square&, const Square&);
template<std::size_t N> Dir2 collition_point (const Circle&, const NEdge<N>&);
template<std::size_t N> Dir2 collition_point (const Square&, const NEdge<N>&);
template<std::size_t N, std::size_t M> Dir2 collition_point (const NEdge<N>&, const NEdge<M>&);



/* * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Implementation needed for template arguments  *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

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


/* * * * * * * * * * * * * *
 *  Function definitions.  *
 * * * * * * * * * * * * * */

template<std::size_t N> 
bool test_collition (const Circle& cir, const NEdge<N>& poly) {
  for (const auto& [A, vB, vC]: poly.triangles) {
    if (test_collition_triangle_circle(A + poly.position, vB, vC, cir.position, cir.radio)) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N> bool test_collition (const Line& line, const NEdge<N>& poly) {
  for (auto [A, vB, vC]: poly.triangles) {
    A += poly.position;
    const Dir2 vL = line.v.percan();
    const float cond1 = vL * A;
    const float cond2 = vL * (A + vB);
    const float cond3 = vL * (A + vC);
    if (cond1*cond2 < 0.f || cond1*cond3 < 0.f) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N> 
bool test_collition (const Particle& par, const NEdge<N>& poly) {
  for (const auto& [A, vB, vC]: poly.triangles) {
    if (test_collition_triangle_circle(A + poly.position, vB, vC, par._position, par._radio)) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N> 
bool test_collition (const Square& sq, const NEdge<N>& poly) {
  Dir2 dims = Dir2 (sq.width, sq.height);
  for (const auto& [A, vB, vC]: poly.triangles) {
    Dir2 At = A + poly.position;
    bool eval1 = test_collition_square_segment (sq.position, dims, At, vB);
    bool eval2 = test_collition_square_segment (sq.position, dims, At, vC);
    bool eval3 = test_collition_square_segment (sq.position, dims, At + vB, vC - vB);
    bool eval4 = test_collition_triangle_point (At, vB, vC, sq.position);
    if (eval1 || eval2 || eval3 || eval4) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N, std::size_t M> 
bool test_collition (const NEdge<N>& poly1, const NEdge<M>& poly2) {
  for (const auto& [A, vB, vC]: poly1.triangles) {
    Dir2 At = A + poly1.position;
    for (const auto& [D, vE, vF]: poly2.triangles) {
      Dir2 Dt = D + poly2.position;
      bool eval1 = test_collition_triangle_segment (At, vB, vC, Dt, vE);
      bool eval2 = test_collition_triangle_segment (At, vB, vC, Dt, vF);
      bool eval3 = test_collition_triangle_segment (At, vB, vC, Dt + vE, vF - vE);
      bool eval4 = test_collition_triangle_point (Dt, vE, vF, At);
      if (eval1 || eval2 || eval3 || eval4) [[unlikely]] {
        return true;
      }
    }
  }
  return false;
}

template<std::size_t N> void correct_collition (Circle& cir, NEdge<N>& pol) {
  std::array<Dir2, N> considered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  for (uint32_t i = 0; i < N; i++)
    lines[i] = std::pair<Dir2, Dir2>{
      pol.points[(i+1)%N] - pol.points[i], 
      pol.points[i] + pol.position
    };
  uint32_t many = 0, filtered = 0;
  Dir2 d = Dir2();

  // testing points that contribute to the direction.
  for (uint32_t i = 0; i < N; i++) {
    const Dir2 P1 = lines[i].second;
    const Dir2 v = lines[i].first;
    const Dir2 coef = Dir2(v * (cir.position - P1) / v.modulo2(), 0.f).bound01();
    const Dir2 Col = v.madd(coef.x, P1) - cir.position;
    const float mod = Col.modulo2();
    if (mod < cir.radio * cir.radio) {
      uint32_t j = 0;
      while (j < many && considered[j] != Col) { j++; }
      if (j == many) {
        d += Col / mod;
        considered[many] = Col;
        many++;
      }
      lines[filtered] = lines[i];
      filtered++;
    }
  }

  // genereate the point base in the direction.
  float distance = 0.f;
  Dir2 dn = d.normalize();
  if (d != Dir2()) {
    Dir2 dL = d.percan().normalize() * (1.f / cir.radio);
    float r2 = cir.radio * cir.radio;
    for (uint32_t i = 0; i < filtered; i++) {
      Dir2 v = lines[i].first;
      Dir2 K1 = lines[i].second;

      Dir2 v2 = v / v.modulo2();
      Dir2 b = cir.position - K1;

      float mult_dist_1 = dL * b;
      if (std::abs(mult_dist_1) < 1.f) [[unlikely]] {
        float q = b * dn;
        float new_distance = -(std::sqrt(r2 - dn.msub(q, b).modulo2()) + q);
        distance = std::min(distance, new_distance);
      }
      
      float coef = std::max(0.f, std::min(1.f, v2 * b));
      Dir2 u1 = cir.position - v.madd(coef, K1);
      float mult_dist_M = dL * u1;
      if (0.0001f < coef && coef < 0.9998f && std::abs(mult_dist_M) < 1.f) [[unlikely]] {
        Dir2 u2_mid_1 = dn.madd((cir.radio - u1.modulo()) / (dn * u1.normalize()), b);
        float u2_mid_2 = std::max(0.f, std::min(1.f, v2 * u2_mid_1));
        Dir2 u2 = v.nmadd(u2_mid_2, b);

        float q = u2 * dn;
        float new_distance = -(std::sqrt(r2 - dn.msub(q, u2).modulo2()) + q);
        distance = std::min(distance, new_distance);
      }
    }
  }

  cir.position = dn.madd(distance, cir.position);
}

template<std::size_t N> Dir2 collition_point (const Circle& cir, const NEdge<N>& pol) {
  std::array<Dir2, N> considered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  for (uint32_t i = 0; i < N; i++)
    lines[i] = std::pair<Dir2, Dir2>{
      pol.points[(i+1)%N] - pol.points[i], 
      pol.points[i] + pol.position
    };
  uint32_t many = 0;
  Dir2 d = Dir2();

  // testing points that contribute to the direction.
  for (uint32_t i = 0; i < N; i++) {
    const Dir2 P1 = lines[i].second;
    const Dir2 v = lines[i].first;
    const Dir2 coef = Dir2(v * (cir.position - P1) / v.modulo2(), 0.f).bound01();
    const Dir2 Col = v.madd(coef.x, P1) - cir.position;
    const float mod = Col.modulo2();
    if (mod < cir.radio * cir.radio) {
      uint32_t j = 0;
      while (j < many && considered[j] != Col) { j++; }
      if (j == many) {
        d += Col / mod;
        considered[many] = Col;
        many++;
      }
    }
  }

  auto test_suitable = [](const float& coef, const float& d_distance) {
    return 
      (0.f < d_distance && -0.0001f < coef && coef < 1.0001f);
  };

  // genereate the point base in the direction.
  Dir2 point = Dir2();
  if (d != Dir2()) {
    Dir2 P1, v;
    int32_t i = -1;
    float coef, d_distance;

    // finding the first suitable point.
    do {
      i++; 
      v = lines[i].first;
      P1 = lines[i].second;
      coef = d.pLd(cir.position - P1, v);
      d_distance = v.pLd(P1 - cir.position, d);
    } while (i < static_cast<int32_t>(N) && !test_suitable(coef, d_distance));
    if (-0.0001f < coef && coef < 1.0001f)
      point = v.madd(coef, P1);

    // find the most suitable point.
    while (i < static_cast<int32_t>(N)) {
      i++; 
      v = lines[i].first;
      P1 = lines[i].second;
      coef = d.pLd(cir.position - P1, v);
      const float new_d_distance = v.pLd(P1 - cir.position, d);
      if (new_d_distance < d_distance && test_suitable(coef, new_d_distance)) {
        point = v.madd(coef, P1);
        d_distance = new_d_distance;
      }
    }
  }

  return point;
}
