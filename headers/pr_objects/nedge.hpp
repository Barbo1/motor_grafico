#pragma once

#include "../concepts/glyph_system.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"

#include <SDL2/SDL.h>
#include <cstdint>
#include <sys/types.h>

struct NEdge {
  MemDir2 position;
  MemDir2 velocity;
  MemDir2 force;
  MemDir2 collision_normal;

  std::array<MemDir2, 3>* triangles;
  std::array<MemDir2, 3>* placed_triangles;
  std::array<MemDir2, 2>* placed_segments;
  uint32_t size;

  float ang_pos;
  float ang_vel;
  float ang_for;

  float area; /* px^2 */
  float density; /* kg/px^2 */
  float f_k; /* kinetic fritction. */
  float acc_f_k;
  float inertia;

  //  0: movible -> The external forces adn velocities don't affect it.
  //  1: normal_presence -> Denote if the collision was made, so the 
  //        force can be corrected and the friction applied.
  //  2...: undefined.
  uint8_t config;

  void print (Global * glb, GlyphsSystem * gs);

  void calculate_movement(Global* glb, const AngDir2 & extrenal_forces);

  void set_position (const AngDir2& center);
  void set_velocity (const AngDir2 & velocity);
  void set_force (const AngDir2 & force);

  void add_force (const AngDir2 & force);
  void add_velocity (const AngDir2 & velocity);

  float get_mass () const;

  /* This function is meant to reposition the polygon in the space to be able 
   * to operate correctly. The structures mantained in for each one provides 
   * the ability to save computation, only calculating when it moves.
   *
   * It assume this->position has been overriden with the new location of the
   * center of the polygon.
   * */
  void reposition_polygon();
  
  void reposition_segments();
};

template<std::size_t N>
struct NEdgeComp {
  std::array<std::array<MemDir2, 3>, N> triangles;
  std::array<std::array<MemDir2, 3>, N-2> placed_triangles;
  std::array<std::array<MemDir2, 2>, N> placed_segments;
  NEdge physical;

  NEdgeComp (
    const Dir2 * points, std::size_t size, AngDir2 center, float density, 
    float f_k = 0, bool movible = true, int* error = nullptr
  ) noexcept;
};


/*
 ----------------------------------------
 |                 Code                 |
 ----------------------------------------
*/


/* This function goes through all of the points in a division part of the polygon, 
 * and fills the 'parts' array with ternary elements, where the first corresponds 
 * to a point of the part, and the remaining two are the vectors going to the 
 * points(the second is the previous, and the third is the continuous).
 * */
static inline int32_t get_parts (
  int32_t first, 
  int32_t last, 
  const Dir2* points, 
  const uint32_t* parts_indexes, 
  std::array<Dir2, 3>* parts
) {

  int32_t parts_size = last - first + 1;

  Dir2 v1 = points[parts_indexes[last]] - points[parts_indexes[first]];
  Dir2 v2 = points[parts_indexes[first + 1]] - points[parts_indexes[first]];
  parts[0] = {points[parts_indexes[first]], v1, v2};

  for (int32_t i = 1; i < parts_size-1; i++) {
    v1 = -v2;
    v2 = points[parts_indexes[first + i + 1]] - points[parts_indexes[first + i]];
    parts[i] = {points[parts_indexes[first + i]], v1, v2};
  }

  v1 = -v2;
  v2 = points[parts_indexes[first]] - points[parts_indexes[last]];
  parts[parts_size-1] = {points[parts_indexes[last]], v1, v2};

  return parts_size;
}

static inline bool are_points_contiguous (int32_t i, int32_t j, int32_t M) {
  int32_t diffi = std::abs (i - j);
  return diffi < 2 || diffi == M;
}


/* * * * * * * * *
 *  Constructor  *
 * * * * * * * * */
template<std::size_t N>
NEdgeComp<N>::NEdgeComp (
  const Dir2 * points, std::size_t size, AngDir2 center, 
  float density, float f_k, bool movible, int* error
) noexcept {
  if (size < N || size < 2) {
    if (error != nullptr)
      *error = -1;
    return;
  }

  // clock order.
  Dir2 mult = Dir2(-1.f, 1.f);
  float order = 0.f;
  for (uint32_t i = 0; i < N; i++) {
    const Dir2 aux = points[i].dir_mul(mult) + points[(i+1)%N];
    order += aux.y() * aux.x();
  }
  bool clockwise = order > 0.f;

  // finding triangles on the polygon.
  std::array<uint32_t, 3*N> parts_indexes;
  uint32_t total_parts = 1;
  {
    bool state_2 = false;

    // structure definitions.
    std::array<std::pair<uint32_t, uint32_t>, N> parts_bounds;
    std::array<std::array<Dir2, 3>, N> parts;
    std::array<uint32_t, 3*N> aux_arr;
    uint32_t parts_bounds_size = 1;
    uint32_t parts_indexes_last = N-1;
    for (uint32_t i = 0; i < N; i++)
      parts_indexes[i] = i;
    parts_bounds[0] = {0, N-1};

    /* * * * * * * * * * * *
     *   Obtuse division   *
     * * * * * * * * * * * */
    while (true) {

      // finding a new non-triangle polygon.
      uint32_t part_index = 0;
      uint32_t first, last;
      int32_t parts_size;
      int32_t index_l = 0, index_f = 0;

      /* State 1: search for point inside a part that has obtuse angle. */
      if (!state_2) {
        for (uint32_t i = 0; i < parts_bounds_size; i++) {
          first = parts_bounds[i].first;
          last = parts_bounds[i].second;
          parts_size = get_parts (first, last, points, parts_indexes.data(), parts.data());
          for (int32_t j = 0; j < parts_size; j++) {
            Dir2 v1 = parts[j][1];
            Dir2 v2 = parts[j][2];
            if ((std::atan2(v2.pL(v1), v1 * v2) > 0.f) ^ clockwise) {
              part_index = i;
              index_f = j;
              goto POLY_STATE_1_END_LOOP;
            }
          }
        }
        state_2 = true;
        continue;
        POLY_STATE_1_END_LOOP:
      
      /* State 2: search for point inside a part with the greatest angle. */
      } else {
        for (uint32_t i = 0; i < parts_bounds_size; i++) {
          first = parts_bounds[i].first;
          last = parts_bounds[i].second;
          if (last-first > 2) {
            part_index = i;
            goto POLY_STATE_2_END_LOOP;
          }
        }
        break;
        POLY_STATE_2_END_LOOP:

        parts_size = get_parts (first, last, points, parts_indexes.data(), parts.data());

        // finding point with greater angle.
        float distance = INFINITY;
        for (int32_t i = 0; i < parts_size; i++) {
          const Dir2 vp = parts[i][1];
          const Dir2 vc = parts[i][2];
          const float new_dist = (vp.normalize() * vc.normalize());
          if ((new_dist > distance) ^ clockwise) {
            distance = new_dist;
            index_f = i;
          }
        }
      }

      // Finding point of division.
      float distance = INFINITY, new_dist;
      std::array<Dir2, 3> part = parts[index_f];
      Dir2 given_point = part[0];
      Dir2 v1 = part[1];
      Dir2 v2 = part[2];
      Dir2 vn = (v1.normalize() + v2.normalize()).normalize().percan();
      for (int32_t i = 0; i < parts_size; i++) {
        Dir2 vec_new_try = parts[i][0] - given_point;
        float c1 = v1.pLd(vec_new_try, v2);
        float c2 = v2.pLd(vec_new_try, v1);
        if (are_points_contiguous (i, index_f, parts_size-1) || ((c1 > 0.0001f && c2 > 0.0001f) ^ state_2))
          continue;

        for (int32_t j = 0; j < parts_size; j++) {
          if (are_points_contiguous (j, index_f, parts_size-1))
            continue;
          Dir2 vec_line = parts[j][2];
          Dir2 diff = parts[j][0] - given_point;

          float c1 = vec_line.pLd(diff, vec_new_try);
          float c2 = -vec_new_try.pLd(diff, vec_line);
          if ((0.0001f < c1 && c1 < 0.9998f && 0.0001f < c2 && c2 < 0.9998f)) {
            goto FIN_DIST_PLOY;
          }
        }

        new_dist = (vn * (vn * vec_new_try)).modulo();
        if (new_dist < distance) {
          distance = new_dist;
          index_l = i;
        }

        FIN_DIST_PLOY:
      }

      if (distance == INFINITY) {
        if (error != nullptr)
          *error = -2;
        return;
      }

      std::pair<uint32_t, uint32_t> max_min = std::minmax(index_l, index_f);
      index_f = max_min.first + first;
      index_l = max_min.second + first;

      // Generating division
        // Changing boundings.
      for (uint32_t i = 0; i < parts_bounds_size; i++) {
        if (parts_bounds[i].first > first) {
          parts_bounds[i].first += 2;
          parts_bounds[i].second += 2;
        }
      }

      uint32_t elem1 = index_l - index_f + 1;
      uint32_t elem2 = last - elem1 + 3;
      parts_bounds[parts_bounds_size++] = {elem2, parts_bounds[part_index].second + 2};
      parts_bounds[part_index].second = elem2-1;

        // Changing indexes.
      uint32_t* indexp = parts_indexes.data();
      uint32_t* auxp = aux_arr.data();
      uint32_t* indexpf = indexp + index_f;
      uint32_t* indexpl = indexp + index_l;
      std::memmove(indexp + last + 3, indexp + last + 1, sizeof(uint32_t) * (parts_indexes_last - last + 1));
      std::memcpy(auxp, indexpf, sizeof(uint32_t) * elem1);
      std::memmove(indexpf + 1, indexpl, sizeof(uint32_t) * (last - index_l + 1));
      std::memcpy(indexp + elem2, auxp, sizeof(uint32_t) * elem1);
      parts_indexes_last += 2;
      total_parts++;
    }

    // proving there are only triangles(for cases with less than 3 points).
    for (uint32_t i = 0; i < parts_bounds_size; i++) {
      uint32_t first = parts_bounds[i].first;
      uint32_t last = parts_bounds[i].second;
      if (last - first != 2) {
        if (error != nullptr)
          *error = -3;
        return;
      }
    }
  }

  if (total_parts != N-2) {
    if (error != nullptr)
      *error = -4;
    return;
  } else *error = 0;
  
  // reorder of the triangles to begin in order.
  uint32_t founded_index = 0;
  std::array<uint32_t, N> founded_many = {};
  std::array<uint32_t, N> founded_where = {};
  while (founded_index < N-2) {
    for (uint32_t i = 3*founded_index; i < 3*(N-2); i++) {
      uint32_t pos = parts_indexes[i];
      founded_where[pos] = i;
      founded_many[pos]++;
    }

    bool founded = false;
    for (uint32_t i = 0; i < N-2 && !founded; i++) {
      if (founded_many[i] == 1) {

        founded = true;
        uint32_t where = founded_where[i];
        uint32_t where_mod = where - ((where) % 3);

        std::swap(parts_indexes[where], parts_indexes[where_mod]);
        for (uint32_t k = 0; k < 3; k++)
          std::swap(parts_indexes[where_mod + k], parts_indexes[3*founded_index + k]);
        founded_index++;
      }
    }
    if (!founded) {
      *error = -5;
      return;
    }

    for (uint32_t i = 0; i < N-2; i++)
      founded_many[i] = 0;
    for (uint32_t i = 0; i < founded_index; i++)
      founded_many[parts_indexes[3*i]] = 2;
  }
  
  // creating the triangles, and calculating final area and inertia.
  this->physical = NEdge {
    .position = MemDir2(),
    .velocity = MemDir2(),
    .force = MemDir2(),
    .collision_normal = MemDir2(),
    .triangles = &(this->triangles[0]),
    .placed_triangles = &(this->placed_triangles[0]),
    .placed_segments = &(this->placed_segments[0]),
    .size = N,
    .ang_pos = center.a(),
    .ang_vel = 0.f,
    .ang_for = 0.f,
    .area = 0.f,
    .density = density,
    .f_k = f_k,
    .acc_f_k = 0.f,
    .inertia = 0.f,
    .config = movible
  };

  this->physical.position.store(center);

  uint32_t* parts_indexes_pointer = parts_indexes.data();
  for (uint32_t i = 0; i < N-2; i++) {
    uint32_t index_1 = *(parts_indexes_pointer++);
    uint32_t index_2 = *(parts_indexes_pointer++);
    uint32_t index_3 = *(parts_indexes_pointer++);
    const Dir2 p1 = points[index_1];
    const Dir2 p2 = points[index_2];
    const Dir2 p3 = points[index_3];
    const Dir2 v1 = p2 - p1;
    const Dir2 v2 = p3 - p1;
    const Dir2 v3 = p3 - p2;

    const float triangle_area = 0.5f * std::abs(v1.pL(v3));
    const float triangle_inertia = (v1.modulo2() + v2.modulo2() + v3.modulo2()) / 36.f;
    const float triangle_inertia_dev = (p1 + p2 + p3).modulo2() / 9.f;
    this->physical.area += triangle_area;
    this->physical.inertia = std::fmaf(triangle_inertia + triangle_inertia_dev, triangle_area, this->physical.inertia);

    this->triangles[index_1][0].store(p1);
    this->triangles[index_1][1].store(v1);
    this->triangles[index_1][2].store(v2);
  }
  this->triangles[N-2][0].store(points[N-2]);
  this->triangles[N-1][0].store(points[N-1]);
  this->physical.inertia *= density;

  this->physical.reposition_polygon();
}


/* * * * * * * * *
 *  Operations  *
 * * * * * * * * */
