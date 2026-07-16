#pragma once

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
bool test_collision (const Line&, const Particle&);
bool test_collision (const Line&, const Circle&);
bool test_collision (const Line&, const Square&);
template<std::size_t N> bool test_collision (const Line&, const NEdge<N>&);

bool test_collision (const Particle&, const Circle&);
bool test_collision (const Particle&, const Square&);
template<std::size_t N> bool test_collision (const Particle&, const NEdge<N>&);

bool test_collision (const Circle&, const Circle&);
bool test_collision (const Circle&, const Square&);
bool test_collision (const Square&, const Square&);
template<std::size_t N> bool test_collision (const Circle&, const NEdge<N>&);
template<std::size_t N> bool test_collision (const Square&, const NEdge<N>&);
template<std::size_t N, std::size_t M> bool test_collision (const NEdge<N>&, const NEdge<M>&);



/* Take two objects and generate the collision. The order 
 * of the parameters are from least to most complex. 
 * */
void resolve_collision (Line&, Particle&);
void resolve_collision (Line&, Circle&);
void resolve_collision (Line&, Square&);
template<std::size_t N> void resolve_collision (Line&, NEdge<N>&);

void resolve_collision (Particle&, Square&);
void resolve_collision (Particle&, Circle&);
template<std::size_t N> void resolve_collision (Particle&, NEdge<N>&);

void resolve_collision (Circle&, Circle&);
void resolve_collision (Square&, Square&);
void resolve_collision (Circle&, Square&, bool);
template<std::size_t N> void resolve_collision (Circle&, NEdge<N>&, bool);
template<std::size_t N> void resolve_collision (Square&, NEdge<N>&, bool);
template<std::size_t N, std::size_t M> void resolve_collision (NEdge<N>&, NEdge<M>&, bool);



/* Given that a collision had ocurred, this function corrects 
 * the superposition of the objects moving the first slightly, 
 * so that the only point in it is the one returned by 
 * collision_point.
 * */

void correct_collision (Line&, Particle&);
void correct_collision (Line&, Square&);
void correct_collision (Line&, Circle&);
template<std::size_t N> void correct_collision (Line&, NEdge<N>&);

void correct_collision (Particle&, Square&);
void correct_collision (Particle&, Circle&);
template<std::size_t N> void correct_collision (Particle&, NEdge<N>&);

void correct_collision (Circle&, Circle&);
void correct_collision (Square&, Square&);
void correct_collision (Circle&, Square&, bool);
template<std::size_t N> void correct_collision (Circle&, NEdge<N>&, bool);
template<std::size_t N> void correct_collision (Square&, NEdge<N>&, bool);
template<std::size_t N, std::size_t M> void correct_collision (NEdge<N>&, NEdge<M>&, bool);



/* Returns the point of the collision between two objects. */
Dir2 collision_point (const Line&, const Square&);
Dir2 collision_point (const Line&, const Circle&);
template<std::size_t N> Dir2 collision_point (const Line&, const NEdge<N>&);

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
    const Dir2 A = Dir2(triangle[0]);
    const Dir2 vB = Dir2(triangle[1]);
    const Dir2 vC = Dir2(triangle[2]);
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
  Dir2 sq_dims = Dir2 (sq.dims);
  Dir2 sq_pos = Dir2 (sq.position);
  for (const auto& triangle: poly.placed_triangles) {
    const Dir2 A = triangle[0];
    const Dir2 vB = triangle[1];
    const Dir2 vC = triangle[2];
    Dir2 coefs;
    bool eval1 = test_collision_square_segment (sq_pos, sq_dims, A, vB, coefs);
    bool eval2 = test_collision_square_segment (sq_pos, sq_dims, A, vC, coefs);
    bool eval3 = test_collision_square_segment (sq_pos, sq_dims, A + vB, vC - vB, coefs);
    bool eval4 = test_collision_triangle_point (A, vB, vC, sq_pos);
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * Functions for correction, collition point and resolution * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* due to the information of the circle and nedge, calculate the direction 
 * of the collision. 
 * */
template<std::size_t N>
static inline Dir2 get_direction_nedge_circle(
  float cir_radio, Dir2 cir_pos, 
  const std::array<std::pair<MemDir2, MemDir2>, N>& placed_points, 
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
    const Dir2 Col = v.madd(coef.x(), P1) - cir_pos;
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
  const std::array<std::pair<MemDir2, MemDir2>, N>& placed_points, 
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
  const std::array<std::pair<Dir2, Dir2>, N>& lines,
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
static inline Dir2 collision_point_circle_nedge(
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

/* Given to the information of the square and nedge, calculate the movement
 * vector to the new position of elements. The vector must be added to
 * move the square, and substracted to move the polygon. A small deviation
 * is added to the final vector to separate them enough before collition
 * testing hit in the next frame.
 *
 * Precondition: d.modulo() != 0
 * */
template<std::size_t N>
static inline Dir2 resposition_direction_square_nedge(
  Dir2 sq_pos, Dir2 sq_dims,
  const std::array<std::pair<MemDir2, MemDir2>, N>& placed_points, 
  const std::array<std::array<MemDir2, 3>, N-2>& placed_triangles,
  Dir2& dn
) {
  dn = calculate_direction_square_nedge (
    sq_pos, sq_dims, 
    placed_triangles
  );

  if (dn.modulo2() == 0.f)
    return dn;

  std::array<std::pair<Dir2, Dir2>, N> segments;
  std::size_t many = 0;
  for (const auto& segment: placed_points) {
    const Dir2 v = Dir2(segment.first);
    const Dir2 P = Dir2(segment.second);
    Dir2 coefs;
    if (test_collision_square_segment(sq_pos, sq_dims, P, v, coefs)) {
      segments[many].first = v;
      segments[many].second = P;
      many = many + 1;
    }
  }

  dn = dn.normalize();
  return dn.dir_mul(
    directional_distance_square_segment (
      sq_pos, 
      sq_dims, 
      dn, 
      segments, 
      many
    )
  );
}

/* Given to the information of the square and nedge, returns a point that
 * represent the point of collition. This point is guarantied to be inside
 * the square, but not the polygon.
 * */
template<std::size_t N>
inline Dir2 collision_point_nedge_square (
  Dir2 sq_pos, Dir2 sq_dims,
  const std::array<std::pair<MemDir2, MemDir2>, N>& placed_points
) {
  Dir2 ret = Dir2();
  float denom = 0.f;

  for (const auto& segment: placed_points) {
    Dir2 E = Dir2(segment.second);
    Dir2 vD = Dir2(segment.first);
    Dir2 coefs;
    if (test_collision_square_segment (sq_pos, sq_dims, E, vD, coefs)) {
      float coefs_sum = coefs.sum();
      ret = vD.madd(coefs_sum * 0.5f, E).madd(coefs_sum, ret);
      denom += coefs_sum;
    }
  }

  return ret / denom;
} 

/* Given to the information of the line and nedge, calculate the movement
 * vector to the new position of nedge(must be added to reach position). 
 * A small deviation is added to the final vector to separate them enough 
 * before collition testing hit in the next frame.
 * */
template<std::size_t N>
static inline Dir2 calculate_reposition_line_nedge (
  const Dir2& line_p, const Dir2& line_v, 
  const Dir2& pol_pos,
  const std::array<std::pair<MemDir2, MemDir2>, N>& placed_points,
  Dir2& dn,
  float& distance
) {
  Dir2 line_v_L = line_v.percan();
  Dir2 d = line_v_L.normalize() * std::copysign(1.f, line_v_L * (line_p - pol_pos));
  dn = -d;
  distance = 0.f;
  for (const auto& part: placed_points) {
    float new_distance = line_v.pLd(line_p - Dir2(part.second), d);
    if (new_distance < distance)
      distance = new_distance;
  }

  return d * (distance - 0.1f);
}

/* Given to the information of the line and nedge, returns the point in
 * the direction of the collision. The returned point is guarantied to be
 * inside the line. If there is no point, the function returns (0.f, 0.f).
 * */
template<std::size_t N>
static inline Dir2 collision_point_line_nedge (
  const Dir2& line_p, const Dir2& line_v, 
  const std::array<std::pair<MemDir2, MemDir2>, N>& placed_points
) {
  Dir2 col;
  uint32_t many = 0;
  for (const auto& part: placed_points) {
    Dir2 v = part.first;
    Dir2 p = part.second;
    float distance = line_v.pLd(line_p - p, v);
    if (0.f < distance && distance < 1.f) {
      col += v.madd(distance, p);
      many++;
    }
  }
  if (many > 0)
    return col / static_cast<float>(many);
  else
    return col;
}



/* * * * * * * * * * */
/* * Circle - NEdge * */
/* * * * * * * * * * */


template<std::size_t N> void resolve_collision (Circle& cir, NEdge<N>& pol, bool move_first) {
  uint32_t filtered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, cir.position, pol.placed_points, lines, filtered
  );

  if (d.modulo2() == 0.f)
    return;

  Dir2 reposition = calculate_reposition_nedge_circle(
    d, cir.radio, cir.position, lines, filtered
  );

  Dir2 collision_point = collision_point_circle_nedge (
    d, cir.position, pol.placed_points
  );

  // calculate resolution.
  const Dir2 dn = d.normalize();

  float inv_cir_mass = 1.f / cir.get_mass();
  float inv_pol_mass = 1.f / pol.get_mass();
  float inv_pol_inertia = 1.f / pol.inertia;

  Dir2 cir_v = cir.velocity;
  Dir2 pol_v = pol.velocity;
  Dir2 pol_r = collision_point - Dir2(pol.position);
  Dir2 pol_v_total = pol_r.percan().madd(pol.ang_vel, pol_v);

  float pol_coef = dn.pL(pol_r);
  Dir2 v_diff = pol_v_total - cir_v;
  Dir2 J = dn * (-2.f * (dn * v_diff) / (
    inv_cir_mass + 
    inv_pol_mass + 
    pol_coef * pol_coef * inv_pol_inertia
  ));

  cir.velocity.store(J.nmadd(inv_cir_mass, cir_v));
  pol.velocity.store(J.madd(inv_pol_mass, pol_v));
  pol.ang_vel = std::fmaf(J.pL(pol_r), inv_pol_inertia, pol.ang_vel);

  if (move_first) {
    cir.position.store(Dir2(cir.position) - reposition);
  } else {
    pol.set_position(AngDir2(Dir2(pol.position) + reposition, pol.ang_pos));
  }
  
  cir.collision_normal.store(dn);
  pol.collision_normal.store(-dn);
  
  cir.acc_f_k = cir.f_k * pol.f_k;
  pol.acc_f_k = cir.acc_f_k;

  cir.config |= PCO_IS_NORMAL;
  pol.config |= PCO_IS_NORMAL;
}

template<std::size_t N> void correct_collision (Circle& cir, NEdge<N>& pol, bool move_first) {
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
  Dir2 reposition = calculate_reposition_nedge_circle(
    d, cir.radio, cir_pos, lines, filtered
  );

  // calculate the reposition distance based on the direction d.
  if (move_first) {
    cir.position.store(cir_pos - reposition);
  } else {
    pol.set_position(AngDir2(reposition, pol.ang_pos));
  }
}

template<std::size_t N> Dir2 collision_point (const Circle& cir, const NEdge<N>& pol) {
  Dir2 cir_pos = cir.position;

  uint32_t filtered;
  std::array<std::pair<Dir2, Dir2>, N> lines;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, cir_pos, pol.placed_points, lines, filtered
  );

  if (d.modulo2() == 0.f)
    return d;

  return collision_point_circle_nedge (
    d, cir_pos, pol.placed_points
  );
}


/* * * * * * * * * * */
/* * Square- NEdge * */
/* * * * * * * * * * */


template<std::size_t N> void correct_collision (Square& sq, NEdge<N>& pol, bool move_first) {
  Dir2 sq_dims = Dir2(sq.dims);
  Dir2 sq_pos = Dir2(sq.position);

  Dir2 dn = Dir2();
  Dir2 collision_direction = resposition_direction_square_nedge(
    sq_pos, sq_dims,
    pol.placed_points, 
    pol.placed_triangles,
    dn
  );

  if (dn.modulo2() == 0.f)
    return;
  
  if (move_first) {
    sq.position.store(sq_pos + collision_direction);
  } else {
    pol.set_position(AngDir2(Dir2(pol.position) - collision_direction, pol.ang_pos));
  }
}

template<std::size_t N> Dir2 collision_point (const Square& sq, const NEdge<N>& pol) {
  return collision_point_nedge_square (sq.position, sq.dims, pol.placed_points);
}

template<std::size_t N> void resolve_collision (Square& sq, NEdge<N>& pol, bool move_first) {
  Dir2 sq_dims = Dir2(sq.dims);
  Dir2 sq_pos = Dir2(sq.position);

  Dir2 dn = Dir2();
  Dir2 collision_direction = resposition_direction_square_nedge(
    sq_pos, 
    sq_dims,
    pol.placed_points, 
    pol.placed_triangles,
    dn
  );

  if (dn.modulo2() == 0.f)
    return;
  Dir2 collision_point = collision_point_nedge_square (sq_pos, sq_dims, pol.placed_points);

  // calculate resolution.
  float inv_sq_mass = 1.f / sq.get_mass();
  float inv_pol_mass = 1.f / pol.get_mass();

  Dir2 pol_pos = Dir2(pol.position);
  Dir2 sq_v = sq.velocity;
  Dir2 pol_v = pol.velocity;
  Dir2 pol_r = collision_point - pol_pos;
  Dir2 pol_v_total = pol_r.percan().madd(pol.ang_vel, pol_v);

  float pol_coef = dn.pL(pol_r);
  Dir2 v_diff = pol_v_total - sq_v;
  float j = -2.f * (dn * v_diff) / (
    inv_sq_mass + 
    inv_pol_mass + 
    pol_coef * pol_coef / pol.inertia 
  );
  Dir2 J = dn * j;

  sq.velocity.store(J.nmadd(inv_sq_mass, sq_v));
  pol.velocity.store(J.madd(inv_pol_mass, pol_v));
  pol.ang_vel = pol.ang_vel + J.pL(pol_r) / pol.inertia;

  if (move_first) {
    sq.position.store(sq_pos + collision_direction);
  } else {
    pol.set_position(AngDir2(pol_pos - collision_direction, pol.ang_pos));
  }
  
  sq.collision_normal.store(dn);
  pol.collision_normal.store(-dn);
  
  sq.acc_f_k = sq.f_k * pol.f_k;
  pol.acc_f_k = sq.acc_f_k;

  sq.config |= PCO_IS_NORMAL;
  pol.config |= PCO_IS_NORMAL;
}


/* * * * * * * * * * */
/* *  Line - NEdge * */
/* * * * * * * * * * */

template<std::size_t N> Dir2 collision_point (const Line& line, const NEdge<N>& pol) {
  return collision_point_line_nedge (
    line.p, line.v, 
    pol.placed_points
  );
}

template<std::size_t N> void correct_collision (Line& line, NEdge<N>& pol) {
  Dir2 pol_pos = Dir2(pol.position), dn;
  Dir2 collision_direction = calculate_reposition_line_nedge (
    line.p, line.v, pol_pos, pol.placed_points, dn
  );

  pol.set_position(AngDir2(collision_direction + pol_pos, pol.ang_pos));
}

template<std::size_t N> void resolve_collision (Line& line, NEdge<N>& pol) {
  Dir2 collision_point = collision_point_line_nedge (
    line.p, line.v, 
    pol.placed_points
  );

  if (collision_point.modulo2() == 0.f)
    return;

  Dir2 pol_pos = pol.position, dn;
  float distance = 0.f;
  Dir2 collision_direction = calculate_reposition_line_nedge (
    line.p, line.v, pol_pos, pol.placed_points, dn, distance
  );

  if (distance == 0.f)
    return;

  // calculate resolution.
  float inv_pol_mass = 1.f / pol.get_mass();
  float inv_pol_inertia = 1.f / pol.inertia;

  Dir2 pol_v = pol.velocity;
  Dir2 pol_r = collision_point - pol_pos;
  Dir2 pol_v_total = pol_r.percan().madd(pol.ang_vel, pol_v);

  float pol_coef = dn.pL(pol_r);
  float j = -2.f * (dn * pol_v_total) / (
    inv_pol_mass + 
    pol_coef * pol_coef * inv_pol_inertia 
  );
  Dir2 J = dn * j;

  pol.velocity.store(J.madd(inv_pol_mass, pol_v));
  pol.ang_vel = std::fmaf(J.pL(pol_r), inv_pol_inertia, pol.ang_vel);

  pol.set_position(AngDir2(pol_pos + collision_direction, pol.ang_pos));
  
  pol.collision_normal.store(-dn);
  pol.acc_f_k = 1.f;
  pol.config |= PCO_IS_NORMAL;
}
