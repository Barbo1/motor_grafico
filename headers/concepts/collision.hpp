#pragma once

#include "./physical.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/nedge.hpp"
#include "../pr_objects/line.hpp"
#include "../pr_objects/particle.hpp"
#include "../primitives/math.hpp"

#include <cstdint>
#include <cmath>


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
    if (test_collision_triangle_circle(Dir2(A), Dir2(vB), Dir2(vC), cir.position, cir.radio)) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N> bool test_collision (const Line& line, const NEdge<N>& poly) {
  for (const auto& triangle: poly.placed_triangles) {
    const Dir2 A = triangle[0];
    const Dir2 vB = triangle[1];
    const Dir2 vC = triangle[2];
    const Dir2 vL = Dir2(line.v).percan();
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
  for (const auto& triangle: poly.placed_triangles) {
    const Dir2 A = triangle[0];
    const Dir2 vB = triangle[1];
    const Dir2 vC = triangle[2];
    if (test_collision_triangle_circle(A, vB, vC, par.position, par.radio)) [[unlikely]] {
      return true;
    }
  }
  return false;
}

template<std::size_t N> 
bool test_collision (const Square& sq, const NEdge<N>& poly) {
  Dir2 dims = Dir2 (sq.dims);
  for (const auto& triangle: poly.placed_triangles) {
    const Dir2 A = triangle[0];
    const Dir2 vB = triangle[1];
    const Dir2 vC = triangle[2];
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
  for (const auto& triangle1: poly1.placed_triangles) {
    const Dir2 A = triangle1[0];
    const Dir2 vB = triangle1[1];
    const Dir2 vC = triangle1[2];
    for (const auto& triangle2: poly2.placed_triangles) {
      const Dir2 D = triangle2[0];
      const Dir2 vE = triangle2[1];
      const Dir2 vF = triangle2[2];
      if (test_collision_triangle_triangle(A, vB, vC, D, vE, vF)) [[unlikely]] {
        return true;
      }
    }
  }
  return false;
}

/* due to the information of the circle and nedge, calculate the direction 
 * of the collision. 
 * */
template<std::size_t N>
static inline Dir2 get_direction_nedge_circle(
  float cir_radio, Dir2 cir_pos, 
  std::array<std::pair<MemDir2, MemDir2>, N> placed_points, 
  std::array<std::pair<Dir2, Dir2>, N>& lines,
  uint32_t& filtered
) {
  filtered = 0;
  uint32_t many = 0; 
  std::array<Dir2, N> considered;
  Dir2 d = Dir2();

  // testing points that contribute to the direction.
  for (uint32_t i = 0; i < N; i++) {
    const Dir2 P1 = placed_points[i].second;
    const Dir2 v = placed_points[i].first;
    const Dir2 coef = Dir2(v * (cir_pos - P1) / v.modulo2(), 0.f).bound01();
    const Dir2 Col = v.madd(coef.x, P1) - cir_pos;
    const float mod = Col.modulo2();
    if (mod < cir_radio * cir_radio) {
      uint32_t j = 0;
      while (j < many && considered[j] != Col) { j++; }
      if (j == many) {
        d += Col / mod;
        considered[many] = Col;
        many++;
      }
      lines[filtered] = placed_points[i];
      filtered++;
    }
  }

  return d;
}

/* Given the information of the circle and nedge, calculate the direction 
 * of the collision. 
 * */
template<std::size_t N>
static inline Dir2 get_direction_nedge_square(
  Dir2 sq_dims, Dir2 sq_pos, 
  std::array<std::pair<MemDir2, MemDir2>, N> placed_points, 
  std::array<std::pair<Dir2, Dir2>, N>& segments,
  uint32_t& many_segments 
) {
  many_segments = 0;
  std::array<Dir2, N> considered;
  uint32_t many_considered = 0; 
  Dir2 d = Dir2();

  // testing points that contribute to the direction.
  for (const auto& line: placed_points) {
    const Dir2 d_part = collision_direction_square_segment (sq_pos, sq_dims, line.second, line.first);
    const float d_part_mod2 = d_part.modulo2();
    if (d_part_mod2 > 0.f) {
      segments[many_segments] = line;
      many_segments++;

      uint32_t j = 0;
      while (j < many_considered && considered[j] != d_part) { j++; }
      if (j == many_considered) {
        d += d_part / d_part_mod2;
        considered[many_considered] = d_part;
        many_considered++;
      }
    }
  }

  return d;
}

/* Given to the information of the circle and nedge, calculate the movement
 * vector to the new position of elements. The vector must be added to
 * move the circle, and substracted to move the polygon. A small deviation
 * is added to the final vector to separate them enough before collition
 * testing hit in the next frame.
 *
 * Precondition: d.modulo() != 0
 * */
template<std::size_t N>
static inline Dir2 calculate_reposition_nedge_circle(
  Dir2 d,
  float cir_radio, Dir2 cir_pos, 
  std::array<std::pair<Dir2, Dir2>, N> lines,
  uint32_t filtered
) {

  // calculate the reposition distance based on the direction d.
  float distance = 0.f;
  const Dir2 dn = d.normalize();
  const Dir2 dL = dn.percan() * (1.f / cir_radio);
  const float r2 = cir_radio * cir_radio;
  for (uint32_t i = 0; i < filtered; i++) {
    const Dir2 v = lines[i].first;
    const Dir2 K1 = lines[i].second;

    const Dir2 v2 = v / v.modulo2();
    const Dir2 b = cir_pos - K1;

    // case 1: the first point of the segment is on the margin.
    const float mult_dist_1 = dL * b;
    if (std::abs(mult_dist_1) < 1.f) [[unlikely]] {
      const float q = b * dn;
      const float new_distance = std::sqrt(r2 - dn.msub(q, b).modulo2()) + q;
      distance = std::max(distance, new_distance);
    }
    
    // case 2: the point in the middle of the segment is on the margin.
    const float coef = std::max(0.f, std::min(1.f, v2 * b));
    const Dir2 u1 = cir_pos - v.madd(coef, K1);
    const float mult_dist_M = dL * u1;
    if (0.0001f < coef && coef < 0.9998f && std::abs(mult_dist_M) < 1.f) [[unlikely]] {
      const Dir2 u2_mid_1 = dn.madd((cir_radio - u1.modulo()) / (dn * u1.normalize()), b);
      const float u2_mid_2 = std::max(0.f, std::min(1.f, v2 * u2_mid_1));
      const Dir2 u2 = v.nmadd(u2_mid_2, b);

      const float q = u2 * dn;
      const float new_distance = std::sqrt(r2 - dn.msub(q, u2).modulo2()) + q;
      distance = std::max(distance, new_distance);
    }
    
    // case 3: the last point don't need calculation, 
    //  the next segment will test it if needed.
  }
  
  return dn * (distance + 0.1f);
}

/* Given to the information of the circle and nedge, returns the point in
 * the direction of the collision(it is not exactly in the place of the
 * collision between the elements).
 *
 * Precondition: d.modulo() != 0
 * */
template<std::size_t N>
static inline Dir2 collision_point_nedge_point_direction(
  Dir2 d, 
  Dir2 position,
  std::array<std::pair<MemDir2, MemDir2>, N>& placed_points
) {

  // genereate the point base in the direction.
  auto test_suitable = [](const float& coef, const float& d_distance) {
    return 
      (0.f < d_distance && -0.0001f < coef && coef < 1.0001f);
  };
  float coef, d_distance, size = static_cast<int32_t>(N-1);
  Dir2 P, v, collision_point;
  int32_t i = -1;

  // finding the first suitable point.
  do {
    i++; 
    v = placed_points[i].first;
    P = placed_points[i].second;
    coef = d.pLd(position - P, v);
    d_distance = v.pLd(P - position, d);
  } while (i < size && !test_suitable(coef, d_distance));
  if (-0.0001f < coef && coef < 1.0001f)
    collision_point = v.madd(coef, P);

  // find the most suitable point.
  while (i < size) {
    i++; 
    v = placed_points[i].first;
    P = placed_points[i].second;
    coef = d.pLd(position - P, v);
    const float new_d_distance = v.pLd(P - position, d);
    if (new_d_distance < d_distance && test_suitable(coef, new_d_distance)) {
      collision_point = v.madd(coef, P);
      d_distance = new_d_distance;
    }
  }

  return collision_point;
}

template<std::size_t N> void resolve_collision (Circle& cir, NEdge<N>& pol) {
  uint32_t filtered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, cir.position, pol.placed_points, lines, filtered
  );

  if (d.modulo2() == 0.f)
    return;

  Dir2 reposition = Dir2(cir.position) - calculate_reposition_nedge_circle(
    d, cir.radio, cir.position, lines, filtered
  );

  Dir2 collision_point = collision_point_nedge_point_direction (
    d, cir.position, pol.placed_points
  );

  // calculate resolution.
  const Dir2 dn = d.normalize();

  float cir_mass = 1.f / cir.get_mass();
  float pol_mass = 1.f / pol.get_mass();
  float pol_inertia = 1.f / pol.inertia;

  Dir2 cir_v = cir.velocity;
  Dir2 pol_vel = pol.velocity;
  Dir2 pol_r = collision_point - Dir2(pol.position);
  Dir2 pol_v = pol_r.percan().madd(pol.ang_vel, pol_vel);

  float pol_coef = dn.pL(pol_r);
  Dir2 v_diff = pol_v - cir_v;
  Dir2 J = dn * (-2.f * (dn * v_diff) / (
    cir_mass + 
    pol_mass + 
    pol_coef * pol_coef * pol_inertia
  ));

  cir.velocity.store(J.nmadd(cir_mass, cir_v));
  pol.velocity.store(J.madd(pol_mass, pol_vel));
  pol.ang_vel = std::fmaf(J.pL(pol_r), pol_inertia, pol.ang_vel);

  cir.position.store(reposition);
  
  cir.collision_normal.store(dn);
  pol.collision_normal.store(-dn);
  
  cir.acc_f_k = cir.f_k * pol.f_k;
  pol.acc_f_k = cir.acc_f_k;

  cir.config |= PCO_IS_NORMAL;
  pol.config |= PCO_IS_NORMAL;
}

template<std::size_t N> void resolve_collision (NEdge<N>& pol, Circle& cir) {
  uint32_t filtered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, cir.position, pol.placed_points, lines,filtered
  );

  if (d.modulo2() == 0.f)
    return;

  Dir2 reposition = pol.position + calculate_reposition_nedge_circle(
    d, cir.radio, cir.position, lines, filtered
  );

  Dir2 collision_point = collision_point_nedge_point_direction (
    d, cir.position, pol.placed_points
  );

  // calculate resolution.
  const Dir2 dn = d.normalize();

  float cir_mass = 1.f / cir.get_mass();
  float pol_mass = 1.f / pol.get_mass();
  float pol_inertia = 1.f / pol._intertia;

  Dir2 cir_v = cir.velocity;
  Dir2 pol_r = collision_point - Dir2(pol.position);
  Dir2 pol_vel = pol._velocity;
  Dir2 pol_v = pol_r.percan().madd(pol._velocity.a, pol_vel);

  float pol_coef = dn.pL(pol_r);
  Dir2 v_diff = pol_v - cir_v;
  Dir2 J = dn * (-2.f * (dn * v_diff) / (
    cir_mass + 
    pol_mass + 
    pol_coef * pol_coef * pol_inertia
  ));

  cir.velocity.store(J.nmadd(cir_mass, cir_v));
  float pol_ang_vel = pol._velocity.a;
  pol._velocity = J.madd(pol_mass, pol_vel);
  pol._velocity.a.store(std::fmaf(J.pL(pol_r), pol_inertia, pol_ang_vel));

  pol.position.set_position(reposition);
  
  cir.collision_normal.store(dn);
  pol.collision_normal.store(-dn);
  
  cir.acc_f_k = cir.f_k * pol.f_k;
  pol.acc_f_k = cir.acc_f_k;

  cir.config |= PCO_IS_NORMAL;
  pol.config |= PCO_IS_NORMAL;
}

template<std::size_t N> void correct_collision (Circle& cir, NEdge<N>& pol) {
  uint32_t filtered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  Dir2 cir_pos = cir.position;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, 
    cir_pos, 
    pol.placed_points, 
    lines,
    filtered
  );

  if (d.modulo2() == 0.f)
    return;

  // calculate the reposition distance based on the direction d.
  cir.position.store(
    cir_pos - calculate_reposition_nedge_circle(
      d, cir.radio, cir.position, lines, filtered
    )
  );
}

template<std::size_t N> void correct_collision (NEdge<N>& pol, Circle& cir) {
  uint32_t filtered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, 
    cir.position, 
    pol.placed_points, 
    lines,
    filtered
  );

  if (d.modulo2() == 0.f)
    return;

  // calculate the reposition distance based on the direction d.
  Dir2 reposition = pol.position + AngDir2(calculate_reposition_nedge_circle(
    d, cir.radio, cir.position, lines, filtered
  ));

  pol.set_position(AngDir2(reposition.x, reposition.y, pol.position.a));
}

template<std::size_t N> Dir2 collision_point (const Circle& cir, const NEdge<N>& pol) {
  uint32_t filtered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, 
    cir.position, 
    pol.placed_points, 
    lines,
    filtered
  );

  if (d.modulo2() == 0.f)
    return d;

  return collision_point_nedge_point_direction (
    d, cir.position, pol.placed_points
  );
}

template<std::size_t N> void correct_collision (Square& sq, NEdge<N>& pol) {
  Dir2 sq_dims = Dir2(sq.dims);
  Dir2 sq_pos = Dir2(sq.position);
  Dir2 d = calculate_direction_square_nedge (
    sq_pos, sq_dims, 
    pol.placed_triangles
  );

  if (d.modulo2() == 0.f)
    return;

  std::array<std::pair<Dir2, Dir2>, N> segments;
  std::size_t many = 0;
  for (const auto& segment: pol.placed_points) {
    const Dir2 v = Dir2(segment.first);
    const Dir2 P = Dir2(segment.second);
    if (test_collision_square_segment(sq_pos, sq_dims, P, v)) {
      segments[many].first = v;
      segments[many].second = P;
      many = many + 1;
    }
  }

  const Dir2 dn = d.normalize();
  sq.position.store(
    sq_pos + dn.dir_mul(
      directional_distance_square_segment (
        sq_pos, 
        sq_dims, 
        dn, 
        segments, 
        many
      )
    )
  );
}

template<std::size_t N> void correct_collision (NEdge<N>& pol, Square& sq) {
  Dir2 sq_dims = Dir2(sq.dims);
  Dir2 sq_pos = Dir2(sq.position);
  Dir2 d = calculate_direction_square_nedge (
    sq_pos, sq_dims, 
    pol.placed_triangles
  );

  if (d.modulo2() == 0.f)
    return;

  std::array<std::pair<Dir2, Dir2>, N> segments;
  std::size_t many = 0;
  for (const auto& segment: pol.placed_points) {
    const Dir2 v = Dir2(segment.first);
    const Dir2 P = Dir2(segment.second);
    if (test_collision_square_segment(sq_pos, sq_dims, P, v)) {
      segments[many].first = v;
      segments[many].second = P;
      many = many + 1;
    }
  }

  const Dir2 dn = d.normalize();
  sq.position.store(
    Dir2(sq.position) - dn.dir_mul(
      directional_distance_square_segment (
        sq_pos, 
        sq_dims, 
        dn, 
        segments, 
        many
      )
    )
  );
}

template<std::size_t N> Dir2 collision_point (const Square& sq, const NEdge<N>& pol) {
  Dir2 sq_dims = Dir2(sq.dims);
  uint32_t many_segments;
  std::array<std::pair<Dir2, Dir2>, N> segments;
  Dir2 d = get_direction_nedge_square (
    sq_dims, sq.position, 
    pol.placed_points, 
    segments,
    many_segments 
  );

  if (d.modulo2() == 0.f)
    return d;
  else return collision_point_nedge_point_direction (
    d.normalize(), sq.position, pol.placed_points
  );
}
