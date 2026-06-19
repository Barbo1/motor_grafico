#pragma once

#include "./physical.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/nedge.hpp"
#include "../pr_objects/line.hpp"
#include "../pr_objects/particle.hpp"
#include "../primitives/math.hpp"
#include <cstdint>


/* Take two objects and verifies if they have collide. The 
 * order of the parameters are from least to most complex. 
 * */
bool test_collision (const Line&, Physical&);
bool test_collision (const Line&, const Particle&);
bool test_collision (const Line&, const Circle&);
bool test_collision (const Line&, const Square&);
template<std::size_t N> bool test_collision (const Line&, const NEdge<N>&);

bool test_collision (const Particle&, Physical&);
bool test_collision (const Particle&, const Circle&);
bool test_collision (const Particle&, const Square&);
template<std::size_t N> bool test_collision (const Particle&, const NEdge<N>&);

bool test_collision (Physical&, Physical&);
bool test_collision (const Circle&, const Circle&);
bool test_collision (const Circle&, const Square&);
bool test_collision (const Square&, const Square&);
template<std::size_t N> bool test_collision (const Circle&, const NEdge<N>&);
template<std::size_t N> bool test_collision (const Square&, const NEdge<N>&);
template<std::size_t N, std::size_t M> bool test_collision (const NEdge<N>&, const NEdge<M>&);



/* Take two objects and generate the collision. The order 
 * of the parameters are from least to most complex. 
 * */
void resolve_collision (Physical&, Line&);
void resolve_collision (Particle&, Line&);
void resolve_collision (Circle&, Line&);
void resolve_collision (Square&, Line&);
template<std::size_t N> void resolve_collision (NEdge<N>&, Line&);

void resolve_collision (Particle&, Physical&);
void resolve_collision (Particle&, Square&);
void resolve_collision (Particle&, Circle&);
template<std::size_t N> void resolve_collision (Particle&, NEdge<N>&);

void resolve_collision (Physical&, Physical&);
void resolve_collision (Circle&, Circle&);
void resolve_collision (Circle&, Square&);
void resolve_collision (Square&, Circle&);
void resolve_collision (Square&, Square&);
template<std::size_t N> void resolve_collision (Circle&, NEdge<N>&);
template<std::size_t N> void resolve_collision (Square&, NEdge<N>&);
template<std::size_t N> void resolve_collision (NEdge<N>&, Circle&);
template<std::size_t N> void resolve_collision (NEdge<N>&, Square&);
template<std::size_t N, std::size_t M> void resolve_collision (NEdge<N>&, NEdge<M>&);



/* Given that a collision had ocurred, this function corrects 
 * the superposition of the objects moving the first slightly, 
 * so that the only point in it is the one returned by 
 * collision_point.
 * */

void correct_collision (Physical&, Line&);
void correct_collision (Particle&, Line&);
void correct_collision (Square&, Line&);
void correct_collision (Circle&, Line&);
template<std::size_t N> void correct_collision (NEdge<N>&, Line&);

void correct_collision (Particle&, Physical&);
void correct_collision (Particle&, Square&);
void correct_collision (Particle&, Circle&);
template<std::size_t N> void correct_collision (Particle&, NEdge<N>&);

void correct_collision (Physical&, Physical&);
void correct_collision (Circle&, Circle&);
void correct_collision (Circle&, Square&);
void correct_collision (Square&, Circle&);
void correct_collision (Square&, Square&);
template<std::size_t N> void correct_collision (Circle&, NEdge<N>&);
template<std::size_t N> void correct_collision (Square&, NEdge<N>&);
template<std::size_t N> void correct_collision (NEdge<N>&, Circle&);
template<std::size_t N> void correct_collision (NEdge<N>&, Square&);
template<std::size_t N, std::size_t M> void correct_collision (NEdge<N>&, NEdge<M>&);



/* Returns the point of the collision between two objects. */
Dir2 collision_point (const Line&, const Physical&);
Dir2 collision_point (const Line&, const Square&);
Dir2 collision_point (const Line&, const Circle&);
template<std::size_t N> Dir2 collision_point (const Line&, const NEdge<N>&);

Dir2 collision_point (const Physical&, const Physical&);
Dir2 collision_point (const Circle&, const Circle&);
Dir2 collision_point (const Circle&, const Square&);
Dir2 collision_point (const Square&, const Square&);
template<std::size_t N> Dir2 collision_point (const Circle&, const NEdge<N>&);
template<std::size_t N> Dir2 collision_point (const Square&, const NEdge<N>&);
template<std::size_t N, std::size_t M> Dir2 collision_point (const NEdge<N>&, const NEdge<M>&);


/* * * * * * * * * * * * * *
 *  Function definitions.  *
 * * * * * * * * * * * * * */

template<std::size_t N> 
bool test_collision (const Circle& cir, const NEdge<N>& poly) {
  for (const auto& [A, vB, vC]: poly.placed_triangles) {
    if (test_collision_triangle_circle(A, vB, vC, cir.position, cir.radio)) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N> bool test_collision (const Line& line, const NEdge<N>& poly) {
  for (auto [A, vB, vC]: poly.placed_triangles) {
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
bool test_collision (const Particle& par, const NEdge<N>& poly) {
  for (const auto& [A, vB, vC]: poly.placed_triangles) {
    if (test_collision_triangle_circle(A, vB, vC, par._position, par._radio)) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N> 
bool test_collision (const Square& sq, const NEdge<N>& poly) {
  Dir2 dims = Dir2 (sq.width, sq.height);
  for (const auto& [A, vB, vC]: poly.placed_triangles) {
    bool eval1 = test_collision_square_segment (sq.position, dims, A, vB);
    bool eval2 = test_collision_square_segment (sq.position, dims, A, vC);
    bool eval3 = test_collision_square_segment (sq.position, dims, A + vB, vC - vB);
    bool eval4 = test_collision_triangle_point (A, vB, vC, sq.position);
    if (eval1 || eval2 || eval3 || eval4) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N, std::size_t M> 
bool test_collision (const NEdge<N>& poly1, const NEdge<M>& poly2) {
  for (const auto& [A, vB, vC]: poly1.placed_triangles) {
    for (const auto& [D, vE, vF]: poly2.placed_triangles) {
      if (test_collision_triangle_triangle(A, vB, vC, D, vE, vF)) [[unlikely]] {
        return true;
      }
    }
  }
  return false;
}

template<std::size_t N> void resolve_collision (Circle& cir, NEdge<N>& pol) {
  std::array<Dir2, N> considered;
  std::array<std::pair<Dir2, Dir2>, N> filtered_lines;

  uint32_t many = 0, filtered = 0;
  Dir2 d = Dir2();

  // testing points that contribute to the direction.
  for (uint32_t i = 0; i < N; i++) {
    const Dir2 P1 = pol.placed_points[i].second;
    const Dir2 v = pol.placed_points[i].first;
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
      filtered_lines[filtered] = pol.placed_points[i];
      filtered++;
    }
  }

  if (d.modulo2() == 0.f)
    return;

  // needed to calculate.
  Dir2 reposition, collision_point = cir.position;

  // calculate the reposition distance based on the direction d.
  float distance = 0.f;
  const Dir2 dn = d.normalize();
  const Dir2 dL = dn.percan() * (1.f / cir.radio);
  const float r2 = cir.radio * cir.radio;
  for (uint32_t i = 0; i < filtered; i++) {
    const Dir2 v = filtered_lines[i].first;
    const Dir2 K1 = filtered_lines[i].second;

    const Dir2 v2 = v / v.modulo2();
    const Dir2 b = cir.position - K1;

    // case 1: the first point of the segment is on the margin.
    const float mult_dist_1 = dL * b;
    if (std::abs(mult_dist_1) < 1.f) [[unlikely]] {
      const float q = b * dn;
      const float new_distance = std::sqrt(r2 - dn.msub(q, b).modulo2()) + q;
      distance = std::max(distance, new_distance);
    }
    
    // case 2: the point in the middle of the segment is on the margin.
    const float coef = std::max(0.f, std::min(1.f, v2 * b));
    const Dir2 u1 = cir.position - v.madd(coef, K1);
    const float mult_dist_M = dL * u1;
    if (0.0001f < coef && coef < 0.9998f && std::abs(mult_dist_M) < 1.f) [[unlikely]] {
      const Dir2 u2_mid_1 = dn.madd((cir.radio - u1.modulo()) / (dn * u1.normalize()), b);
      const float u2_mid_2 = std::max(0.f, std::min(1.f, v2 * u2_mid_1));
      const Dir2 u2 = v.nmadd(u2_mid_2, b);

      const float q = u2 * dn;
      const float new_distance = std::sqrt(r2 - dn.msub(q, u2).modulo2()) + q;
      distance = std::max(distance, new_distance);
    }
    
    // case 3: the last point don't need calculation, 
    //  the next segment will test it if needed.
  }
  reposition = dn.nmadd(distance + 0.1f, cir.position);


  // genereate the point base in the direction.
  auto test_suitable = [](const float& coef, const float& d_distance) {
    return 
      (0.f < d_distance && -0.0001f < coef && coef < 1.0001f);
  };
  Dir2 P1, v;
  int32_t i = -1;
  float coef, d_distance;

  // finding the first suitable point.
  do {
    i++; 
    v = pol.placed_points[i].first;
    P1 = pol.placed_points[i].second;
    coef = d.pLd(cir.position - P1, v);
    d_distance = v.pLd(P1 - cir.position, d);
  } while (i < static_cast<int32_t>(N) && !test_suitable(coef, d_distance));
  if (-0.0001f < coef && coef < 1.0001f)
    collision_point = v.madd(coef, P1);

  // find the most suitable point.
  while (i < static_cast<int32_t>(N)) {
    i++; 
    v = pol.placed_points[i].first;
    P1 = pol.placed_points[i].second;
    coef = d.pLd(cir.position - P1, v);
    const float new_d_distance = v.pLd(P1 - cir.position, d);
    if (new_d_distance < d_distance && test_suitable(coef, new_d_distance)) {
      collision_point = v.madd(coef, P1);
      d_distance = new_d_distance;
    }
  }

  // calculate resolution.
  float cir_mass = 1.f / cir.get_mass();
  float pol_mass = 1.f / pol.get_mass();
  float pol_inertia = 1.f / pol._intertia;

  Dir2 cir_v = cir._velocity;
  Dir2 pol_r = collision_point - pol.position;
  Dir2 pol_v = pol_r.percan().madd(pol._velocity.a, pol._velocity);

  float pol_coef = dn.pL(pol_r);
  Dir2 v_diff = pol_v - cir_v;
  Dir2 J = dn * (-2.f * (dn * v_diff) / (
    cir_mass + 
    pol_mass + 
    pol_coef * pol_coef * pol_inertia
  ));

  cir._velocity = J.nmadd(cir_mass, cir._velocity);
  float pol_ang_vel = pol._velocity.a;
  pol._velocity = J.madd(pol_mass, pol._velocity);
  pol._velocity.a = std::fmaf(J.pL(pol_r), pol_inertia, pol_ang_vel);

  cir.position = reposition;
  
  cir._collision_normal = dn;
  pol._collision_normal = -dn;
  
  cir._acc_f_k = cir._f_k * pol._f_k;
  pol._acc_f_k = cir._acc_f_k;
  
  cir._normal_presence = true;
  pol._normal_presence = true;
}

template<std::size_t N> void correct_collision (Circle& cir, NEdge<N>& pol) {
  Dir2 d = Dir2(0.f, 0.f);
  uint32_t many = 0, filtered = 0;
  std::array<Dir2, N> considered;
  std::array<std::pair<Dir2, Dir2>, N> lines;

  // testing points that contribute to the direction.
  for (uint32_t i = 0; i < N; i++) {
    const Dir2 P1 = pol.placed_points[i].second;
    const Dir2 v = pol.placed_points[i].first;
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
      lines[filtered] = pol.placed_points[i];
      filtered++;
    }
  }

  // calculate the reposition distance based on the direction d.
  float distance = 0.f;
  const Dir2 dn = d.normalize();
  if (d != Dir2()) {
    const Dir2 dL = dn.percan() * (1.f / cir.radio);
    const float r2 = cir.radio * cir.radio;
    for (uint32_t i = 0; i < filtered; i++) {
      const Dir2 v = lines[i].first;
      const Dir2 K1 = lines[i].second;

      const Dir2 v2 = v / v.modulo2();
      const Dir2 b = cir.position - K1;

      // case 1: the first point of the segment is on the margin.
      const float mult_dist_1 = dL * b;
      if (std::abs(mult_dist_1) < 1.f) [[unlikely]] {
        const float q = b * dn;
        const float new_distance = std::sqrt(r2 - dn.msub(q, b).modulo2()) + q;
        distance = std::max(distance, new_distance);
      }
      
      // case 2: the point in the middle of the segment is on the margin.
      const float coef = std::max(0.f, std::min(1.f, v2 * b));
      const Dir2 u1 = cir.position - v.madd(coef, K1);
      const float mult_dist_M = dL * u1;
      if (0.0001f < coef && coef < 0.9998f && std::abs(mult_dist_M) < 1.f) [[unlikely]] {
        const Dir2 u2_mid_1 = dn.madd((cir.radio - u1.modulo()) / (dn * u1.normalize()), b);
        const float u2_mid_2 = std::max(0.f, std::min(1.f, v2 * u2_mid_1));
        const Dir2 u2 = v.nmadd(u2_mid_2, b);

        const float q = u2 * dn;
        const float new_distance = std::sqrt(r2 - dn.msub(q, u2).modulo2()) + q;
        distance = std::max(distance, new_distance);
      }
      
      // case 3: the last point don't need calculation, 
      //  the next segment will test it if needed.
    }
  }

  cir.position = dn.nmadd(distance, cir.position);
}

template<std::size_t N> Dir2 collision_point (const Circle& cir, const NEdge<N>& pol) {
  std::array<Dir2, N> considered;
  uint32_t many = 0;
  Dir2 d = Dir2();

  // testing points that contribute to the direction.
  for (uint32_t i = 0; i < N; i++) {
    const Dir2 P1 = pol.placed_points[i].second;
    const Dir2 v = pol.placed_points[i].first;
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
      v = pol.placed_points[i].first;
      P1 = pol.placed_points[i].second;
      coef = d.pLd(cir.position - P1, v);
      d_distance = v.pLd(P1 - cir.position, d);
    } while (i < static_cast<int32_t>(N) && !test_suitable(coef, d_distance));
    if (-0.0001f < coef && coef < 1.0001f)
      point = v.madd(coef, P1);

    // find the most suitable point.
    while (i < static_cast<int32_t>(N)) {
      i++; 
      v = pol.placed_points[i].first;
      P1 = pol.placed_points[i].second;
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
