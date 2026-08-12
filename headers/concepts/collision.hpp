#pragma once

#include "../pr_objects/square.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/nedge.hpp"
#include "../pr_objects/line.hpp"
#include "../pr_objects/particle.hpp"
#include "../pr_objects/projectile.hpp"
#include "../primitives/math.hpp"

#include <cstdint>
#include <cmath>


/* Take two objects and verifies if they have collide. The 
 * order of the parameters are from least to most complex. 
 * */
bool test_collision (const Line&, const Particle&);
bool test_collision (const Line&, const Circle&);
bool test_collision (const Line&, const Square&);
bool test_collision (const Line&, const NEdge&);

bool test_collision (const Particle&, const Circle&);
bool test_collision (const Particle&, const Square&);
bool test_collision (const Particle&, const NEdge&);

bool test_collision (const Circle&, const Circle&);
bool test_collision (const Circle&, const Square&);
bool test_collision (const Square&, const Square&);
bool test_collision (const Circle&, const NEdge&);
bool test_collision (const Square&, const NEdge&);
bool test_collision (const NEdge&, const NEdge&);

bool test_collision (Projectile&, Projectile&);
bool test_collision (Projectile&, Line&);
bool test_collision (Projectile&, Square&);
bool test_collision (Projectile&, Circle&);
bool test_collision (Projectile&, NEdge&);


/* Take two objects and generate the collision. The order 
 * of the parameters are from least to most complex. 
 * */
void resolve_collision (Line&, Particle&);
void resolve_collision (Line&, Circle&);
void resolve_collision (Line&, Square&);
void resolve_collision (Line&, NEdge&);

void resolve_collision (Particle&, Square&);
void resolve_collision (Particle&, Circle&);
void resolve_collision (Particle&, NEdge&);

void resolve_collision (Circle&, Circle&);
void resolve_collision (Square&, Square&);
void resolve_collision (Circle&, Square&, bool);
void resolve_collision (Circle&, NEdge&, bool);
void resolve_collision (Square&, NEdge&, bool);
void resolve_collision (NEdge&, NEdge&, bool);

bool resolve_collision (Projectile&);


/* Given that a collision had ocurred, this function corrects 
 * the superposition of the objects moving the first slightly, 
 * so that the only point in it is the one returned by 
 * collision_point.
 * */

void correct_collision (Line&, Particle&);
void correct_collision (Line&, Square&);
void correct_collision (Line&, Circle&);
void correct_collision (Line&, NEdge&);

void correct_collision (Particle&, Square&);
void correct_collision (Particle&, Circle&);
void correct_collision (Particle&, NEdge&);

void correct_collision (Circle&, Circle&);
void correct_collision (Square&, Square&);
void correct_collision (Circle&, Square&, bool);
void correct_collision (Circle&, NEdge&, bool);
void correct_collision (Square&, NEdge&, bool);
void correct_collision (NEdge&, NEdge&, bool);

bool correct_collision (Projectile&);


/* Returns the point of the collision between two objects. */
Dir2 collision_point (Line&, Square&);
Dir2 collision_point (Line&, Circle&);
Dir2 collision_point (Line&, NEdge&);

Dir2 collision_point (Circle&, Circle&);
Dir2 collision_point (Circle&, Square&);
Dir2 collision_point (Square&, Square&);
Dir2 collision_point (Circle&, NEdge&);
Dir2 collision_point (Square&, NEdge&);
Dir2 collision_point (NEdge&, NEdge&);

Dir2 collision_point (Projectile&);


/* Returns the direction of the collision between two objects. */
Dir2 collision_direction (Line&, Square&);
Dir2 collision_direction (Line&, Circle&);
Dir2 collision_direction (Line&, NEdge&);

Dir2 collision_direction (Circle&, Circle&);
Dir2 collision_direction (Circle&, Square&);
Dir2 collision_direction (Square&, Square&);
Dir2 collision_direction (Circle&, NEdge&);
Dir2 collision_direction (Square&, NEdge&);
Dir2 collision_direction (NEdge&, NEdge&);

Dir2 collision_direction (Projectile&);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * Functions for correction, collition point and resolution * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* due to the information of the circle and nedge, calculate the direction 
 * of the collision. 
 * */
inline Dir2 get_direction_nedge_circle(
  float cir_radio, 
  Dir2 cir_pos, 
  std::array<MemDir2, 2>* placed_segments, 
  uint32_t size,
  uint32_t& filtered
) {
  filtered = 0;
  Dir2 prev_col = Dir2();
  Dir2 d = Dir2();

  // testing points that contribute to the direction.
  for (uint32_t i = 0; i < size; i++) {
    const Dir2 P1 = placed_segments[i][1];
    const Dir2 v = placed_segments[i][0];
    const Dir2 coef = Dir2(v * (cir_pos - P1) / v.modulo2(), 0.f).bound01();
    const Dir2 Col = v.madd(coef.x(), P1) - cir_pos;
    const float mod = Col.modulo2();
    if (mod < cir_radio * cir_radio) {
      if (prev_col != Col)
        d += Col / mod;

      prev_col = Col;
      std::swap(placed_segments[filtered], placed_segments[i]);
      filtered++;
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
inline Dir2 calculate_reposition_nedge_circle(
  Dir2 d,
  float cir_radio, Dir2 cir_pos, 
  std::array<MemDir2, 2>* lines,
  uint32_t filtered
) {

  // calculate the reposition distance based on the direction d.
  float distance = 0.f;
  Dir2 dn = d.normalize();
  Dir2 dL = dn.percan() * (1.f / cir_radio);
  float r2 = cir_radio * cir_radio;
  for (uint32_t i = 0; i < filtered; i++) {
    Dir2 v = lines[i][0];
    Dir2 K1 = lines[i][1];

    Dir2 v2 = v / v.modulo2();
    Dir2 b = cir_pos - K1;

    // case 1: the first point of the segment is on the margin.
    const float mult_dist_1 = dL * b;
    if (std::abs(mult_dist_1) < 1.f) [[unlikely]] {
      float q = b * dn;
      float new_distance = std::sqrt(r2 - dn.msub(q, b).modulo2()) + q;
      distance = std::max(distance, new_distance);
    }
    
    // case 2: the point in the middle of the segment is on the margin.
    float coef = std::max(0.f, std::min(1.f, v2 * b));
    Dir2 u1 = cir_pos - v.madd(coef, K1);
    float mult_dist_M = dL * u1;
    if (0.0001f < coef && coef < 0.9998f && std::abs(mult_dist_M) < 1.f) [[unlikely]] {
      Dir2 u2_mid_1 = dn.madd((cir_radio - u1.modulo()) / (dn * u1.normalize()), b);
      float u2_mid_2 = std::max(0.f, std::min(1.f, v2 * u2_mid_1));
      Dir2 u2 = v.nmadd(u2_mid_2, b);

      float q = u2 * dn;
      float new_distance = std::sqrt(r2 - dn.msub(q, u2).modulo2()) + q;
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
inline Dir2 collision_point_circle_nedge(
  Dir2 d, 
  Dir2 position,
  std::array<MemDir2, 2>* placed_segments,
  uint32_t many_segments
) {

  // genereate the point base in the direction.
  auto test_suitable = [](const float& coef, const float& d_distance) {
    return 
      (0.f < d_distance && -0.0001f < coef && coef < 1.0001f);
  };
  float coef, d_distance, size = static_cast<int32_t>(many_segments-1);
  Dir2 P, v, collision_point;
  int32_t i = -1;

  // finding the first suitable point.
  do {
    i++; 
    v = placed_segments[i][0];
    P = placed_segments[i][1];
    coef = d.pLd(position - P, v);
    d_distance = v.pLd(P - position, d);
  } while (i < size && !test_suitable(coef, d_distance));
  if (-0.0001f < coef && coef < 1.0001f)
    collision_point = v.madd(coef, P);

  // find the most suitable point.
  while (i < size) {
    i++; 
    v = placed_segments[i][0];
    P = placed_segments[i][1];
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
inline Dir2 resposition_direction_square_nedge(
  Dir2 sq_pos, Dir2 sq_dims,
  std::array<MemDir2, 2>* placed_segments, 
  std::array<MemDir2, 3>* placed_triangles,
  uint32_t size,
  Dir2& dn
) {
  dn = calculate_direction_square_nedge (
    sq_pos, sq_dims, size-2,
    placed_triangles
  );

  if (dn.modulo2() == 0.f)
    return dn;

  std::size_t many = 0;
  for (uint32_t i = 0; i < size; i++) {
    const auto& segment = placed_segments[i];
    const Dir2 v = Dir2(segment[0]);
    const Dir2 P = Dir2(segment[1]);
    Dir2 coefs;
    if (test_collision_square_segment(sq_pos, sq_dims, P, v, coefs)) {
      std::swap(placed_segments[i], placed_segments[many]);
      many = many + 1;
    }
  }

  dn = dn.normalize();
  return dn.dir_mul(
    directional_distance_square_segment (
      sq_pos, 
      sq_dims, 
      dn, 
      placed_segments, 
      many
    )
  );
}

/* Given to the information of the square and nedge, returns a point that
 * represent the point of collition. This point is guarantied to be inside
 * the square, but not the polygon.
 * */
#include <iostream>
inline Dir2 collision_point_nedge_square (
  Dir2 sq_pos, Dir2 sq_dims,
  std::array<MemDir2, 2>* placed_segments,
  uint32_t size
) {
  Dir2 ret = Dir2();
  float denom = 0.f;

  for (uint32_t i = 0; i < size; i++) {
  const auto& segment = placed_segments[i];
    Dir2 E = Dir2(segment[1]);
    Dir2 vD = Dir2(segment[0]);
    Dir2 coefs;
    if (test_collision_square_segment (sq_pos, sq_dims, E, vD, coefs)) {
      std::cout << coefs.x() << ", " << coefs.y() << std::endl;
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
inline Dir2 calculate_reposition_line_nedge (
  const Dir2& line_p, const Dir2& line_v, 
  const Dir2& pol_pos,
  std::array<MemDir2, 2>* placed_segments,
  uint32_t size,
  Dir2& dn,
  float& distance
) {
  Dir2 line_v_L = line_v.percan();
  Dir2 d = line_v_L.normalize() * std::copysign(1.f, line_v_L * (line_p - pol_pos));
  dn = -d;
  distance = 0.f;
  for (uint32_t i = 0; i < size; i++) {
    const auto& part = placed_segments[i];
    float new_distance = line_v.pLd(line_p - Dir2(part[1]), d);
    if (new_distance < distance)
      distance = new_distance;
  }

  return d * (distance - 0.1f);
}

/* Given to the information of the line and nedge, returns the point in
 * the direction of the collision. The returned point is guarantied to be
 * inside the line. If there is no point, the function returns (0.f, 0.f).
 * */
inline Dir2 collision_point_line_nedge (
  const Dir2& line_p, const Dir2& line_v, 
  std::array<MemDir2, 2>* placed_segments,
  uint32_t size
) {
  Dir2 col;
  uint32_t many = 0;
  for (uint32_t i = 0; i < size; i++) {
    const auto& part = placed_segments[i];
    Dir2 v = part[0];
    Dir2 p = part[1];
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
