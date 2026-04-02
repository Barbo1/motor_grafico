#pragma once

#include "./physical.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/nedge.hpp"
#include "../pr_objects/line.hpp"
#include "../pr_objects/particle.hpp"


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
Dir2 collition_point (Line&, Physical&);
Dir2 collition_point (Line&, Square&);
Dir2 collition_point (Line&, Circle&);
template<std::size_t N> Dir2 collition_point (Line&, NEdge<N>&);

Dir2 collition_point (Physical&, Physical&);
Dir2 collition_point (Circle&, Circle&);
Dir2 collition_point (Circle&, Square&);
Dir2 collition_point (Square&, Square&);
template<std::size_t N> Dir2 collition_point (Circle&, NEdge<N>&);
template<std::size_t N> Dir2 collition_point (Square&, NEdge<N>&);
template<std::size_t N, std::size_t M> Dir2 collition_point (NEdge<N>&, NEdge<M>&);


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
  bool c1 = vB.pLd (vE, vC) > 0.f;
  bool c2 = vC.pLd (vE, vB) > 0.f;
  bool c3 = c1 + c2 < 0.f;
  Dir2 vAD = A - D;
  float v1 = vB.pLd(vAD, vE);
  float v2 = vC.pLd(vAD, vE);
  float v3 = (vC - vB).pLd(vB - vAD, vE);

  float c_I = std::max (c1 ? v1 : 0.f, std::max (c2 ? v2 : 0.f, c3 ? v3 : 0.f));
  float c_S = std::min (!c1 ? v1 : 1.f, std::min (!c2 ? v2 : 1.f, !c3 ? v3 : 1.f));

  return c_S > c_I;
}

template<std::size_t N> 
bool test_collition (const Circle& cir, const NEdge<N>& poly) {
  for (const auto& [A, vB, vC]: poly.triangles) {
    if (test_collition_triangle_circle(A + poly.position, vB, vC, cir.position, cir.radio)) {
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
    if (cond1*cond2 < 0.f || cond1*cond3 < 0.f) {
      return true;
    }
  }
  return false;
}

template<std::size_t N> 
bool test_collition (const Particle& par, const NEdge<N>& poly) {
  for (const auto& [A, vB, vC]: poly.triangles) {
    if (test_collition_triangle_circle(A + poly.position, vB, vC, par._position, par._radio)) {
      return true;
    }
  }
  return false;
}
