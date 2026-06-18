#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"
#include "../concepts/glyph_system.hpp"
#include "../concepts/image_modifier.hpp"

#include <SDL2/SDL.h>
#include <codecvt>
#include <cstdint>

class Circle;
class Line;
class Particle;
class Square;

template<std::size_t N>
class NEdge final: public Physical {
  private:
    std::array<Dir2, N> points;
    std::array<std::pair<Dir2, Dir2>, N> placed_points;
    std::array<std::array<Dir2, 3>, N-2> triangles;
    std::array<std::array<Dir2, 3>, N-2> placed_triangles;
    Visualizer<D2FIG> texture;
    Global* glb;

    void reposition_polygon();

  public: 
    NEdge (
      Global* glb, const Dir2 * points, std::size_t size, AngDir2 center, 
      float density = 0, float f_k = 0, bool movible = true, bool colidable = true,
      SDL_Color* color = nullptr, int* error = nullptr
    ) noexcept;
    NEdge (const NEdge &) noexcept;
    NEdge & operator= (const NEdge &) noexcept;

    void set_texture (Visualizer<D2FIG>);
    Visualizer<D2FIG> get_texture () const;

    void print (Global * glb, GlyphsSystem * gs);

    void draw ();

    virtual void calculate_movement(const AngDir2 & extrenal_forces);
    virtual void set_position (AngDir2);

    template<std::size_t M> friend bool test_collition (const Line&, const NEdge<M>&);
    template<std::size_t M> friend bool test_collition (const Particle&, const NEdge<M>&);
    template<std::size_t M> friend bool test_collition (const Circle&, const NEdge<M>&);
    template<std::size_t M> friend bool test_collition (const Square&, const NEdge<M>&);
    template<std::size_t M, std::size_t L> friend bool test_collition (const NEdge<M>&, const NEdge<L>&);

    template<std::size_t M> friend void resolve_collition (NEdge<M>&, Line&);
    template<std::size_t M> friend void resolve_collition (Particle&, NEdge<M>&);
    template<std::size_t M> friend void resolve_collition (Circle&, NEdge<M>&);
    template<std::size_t M> friend void resolve_collition (Square&, NEdge<M>&);
    template<std::size_t M> friend void resolve_collition (NEdge<M>&, Circle&);
    template<std::size_t M> friend void resolve_collition (NEdge<M>&, Square&);
    template<std::size_t M, std::size_t T> friend void resolve_collition (NEdge<M>&, NEdge<T>&);

    template<std::size_t M> friend void correct_collition (NEdge<M>&, Line&);
    template<std::size_t M> friend void correct_collition (Particle&, NEdge<M>&);
    template<std::size_t M> friend void correct_collition (Circle&, NEdge<M>&);
    template<std::size_t M> friend void correct_collition (Square&, NEdge<M>&);
    template<std::size_t M> friend void correct_collition (NEdge<M>&, Circle&);
    template<std::size_t M> friend void correct_collition (NEdge<M>&, Square&);
    template<std::size_t M, std::size_t T> friend void correct_collition (NEdge<M>&, NEdge<N>&);

    template<std::size_t M> friend Dir2 collition_point (const Line&, const NEdge<M>&);
    template<std::size_t M> friend Dir2 collition_point (const Circle&, const NEdge<M>&);
    template<std::size_t M> friend Dir2 collition_point (const Square&, const NEdge<M>&);
    template<std::size_t M, std::size_t T> friend Dir2 collition_point (const NEdge<M>&, const NEdge<T>&);
};


/*
 ----------------------------------------
 |                 Code                 |
 ----------------------------------------
*/

/* This function is meant to reposition the polygon in the space to be able 
 * to operate correctly. The structures mantained in for each one provides 
 * the ability to save computation, only calculating when it moves.
 *
 * It assume this->position has been overriden.
 * */
template<std::size_t N>
void NEdge<N>::reposition_polygon() {
  for (uint32_t i = 0; i < N; i++)
    this->placed_points[i] = std::pair{this->points[(i+1)%N] - this->points[i], this->points[i]};
  rotate_segments(this->placed_points.data(), N, this->position.a);
  for (uint32_t i = 0; i < N; i++)
    this->placed_points[i].second += this->position;

  for (uint32_t i = 0; i < N-2; i++)
    this->placed_triangles[i] = {this->triangles[i][0], this->triangles[i][1], this->triangles[i][2]};
  rotate_triangles(this->placed_triangles.data(), N-2, this->position.a);
  for (uint32_t i = 0; i < N-2; i++)
    this->placed_triangles[i][0] += this->position;
}


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
NEdge<N>::NEdge (
  Global* glb, const Dir2 * points, std::size_t size, AngDir2 center, 
  float density, float f_k, bool movible, bool colidable,
  SDL_Color* color, int* error
) noexcept:
  Physical(glb, center, density, 0.f, f_k, movible, colidable)
{
  // Global* glb, AngDir2 position, float density, float area, float f_k, bool movible, bool colidable
  if (size < N) {
    if (error != nullptr)
      *error = -1;
    return;
  }

  this->glb = glb;

  // writing points.
  for (uint32_t i = 0; i < N; i++)
    this->points[i] = points[i];

  // clock order.
  Dir2 mult = Dir2(-1.f, 1.f);
  Dir2 mass_center = points[N-1];
  float order = 0.f;
  for (uint32_t i = 0; i < N; i++) {
    mass_center += points[i];
    const Dir2 aux = points[i].dir_mul(mult) + points[(i+1)%N];
    order += aux.y * aux.x;
  }
  mass_center *= 1.f / static_cast<float>(N);
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

  // creating the triangles, and calculating final area and inertia.
  uint32_t* parts_indexes_pointer = parts_indexes.data();
  float inertia_distance_acc = 0.f;
  float inertia_deviation_acc = 0.f;
  this->_area = 0.f;
  for (uint32_t i = 0; i < N-2; i++) {
    const Dir2 p1 = points[*(parts_indexes_pointer++)];
    const Dir2 p2 = points[*(parts_indexes_pointer++)];
    const Dir2 p3 = points[*(parts_indexes_pointer++)];
    const Dir2 v1 = p2 - p1;
    const Dir2 v2 = p3 - p1;
    const Dir2 v3 = p3 - p2;

    const float triangle_mass = 0.5f * this->_density * std::abs(v1.pL(v3));
    this->_area += triangle_mass;
    inertia_deviation_acc += ((p1 + p2 + p3) / 3.f - mass_center).modulo2() * triangle_mass;
    inertia_distance_acc += (v1.modulo2() + v2.modulo2() + v3.modulo2()) * triangle_mass;

    this->triangles[i] = {p1, v1, v2};
  }
  this->_intertia = 
    (inertia_distance_acc / 36.f + inertia_deviation_acc) + 
    (mass_center.modulo2() * this->_area * this->_density);
   
  this->reposition_polygon();
  if (color != nullptr) {
    this->texture = ImageModifier::polygon(points, size, *color).cast(glb);
  }
}

/* * * * * * * * * * * *
 *  Copy constructor  *
 * * * * * * * * * * * */

template<std::size_t N>
NEdge<N>::NEdge (const NEdge & poly) noexcept: Physical(poly) {
  this->texture = poly.texture;
  this->points = poly.points;
  this->placed_points = poly.placed_points;
  this->triangles = poly.triangles;
  this->placed_triangles = poly.placed_triangles;
  this->glb = poly.glb;
}


/* * * * * * * * * * *
 *  Copy assignment  *
 * * * * * * * * * * */

template<std::size_t N>
NEdge<N> & NEdge<N>::operator= (const NEdge & poly) noexcept {
  this->texture = poly.texture;
  this->points = poly.points;
  this->placed_points = poly.placed_points;
  this->triangles = poly.triangles;
  this->placed_triangles = poly.placed_triangles;
  this->glb = poly.glb;
  this->Physical::operator=(poly);
  return *this;
}


/* * * * * * * * *
 *  Operations  *
 * * * * * * * * */

template<std::size_t N>
void NEdge<N>::draw () {
  this->texture.draw(glb, this->position);
}

template<std::size_t N>
void NEdge<N>::set_texture (Visualizer<D2FIG> tex) {
  this->texture = tex;
}

template<std::size_t N>
Visualizer<D2FIG> NEdge<N>::get_texture () const {
  return this->texture;
}

template<std::size_t N>
void NEdge<N>::print (Global * glb, GlyphsSystem * gs) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  uint32_t i = 0;
  for (const auto& triangle: this->placed_triangles) {
    const Dir2 point1 = triangle[0];
    const Dir2 point2 = triangle[1] + triangle[0];
    const Dir2 point3 = triangle[2] + triangle[0];
    SDL_SetRenderDrawColor (glb->get_render(), 255, 0, 0, 255);
    SDL_RenderDrawLine (
      glb->get_render(), 
      static_cast<uint32_t>(point1.x),
      static_cast<uint32_t>(point1.y),
      static_cast<uint32_t>(point2.x),
      static_cast<uint32_t>(point2.y)
    );
    SDL_RenderDrawLine (
      glb->get_render(), 
      static_cast<uint32_t>(point1.x),
      static_cast<uint32_t>(point1.y),
      static_cast<uint32_t>(point3.x),
      static_cast<uint32_t>(point3.y)
    );
    SDL_RenderDrawLine (
      glb->get_render(), 
      static_cast<uint32_t>(point2.x),
      static_cast<uint32_t>(point2.y),
      static_cast<uint32_t>(point3.x),
      static_cast<uint32_t>(point3.y)
    );
    gs->print(
      conv.from_bytes(std::to_string(i)), 
      20, 
      SDL_Color {.r = 255, .g = 255, .b = 255, .a = 255}, 
      point2 + Dir2(5.f, 5.f)
    );
    i++;
  }
}

template<std::size_t N>
void NEdge<N>::calculate_movement(const AngDir2 & extrenal_forces) {
  if (this->_movible) {
    AngDir2 final_force = this->_force + extrenal_forces;
    if (this->_normal_presence) {
      this->_normal_presence = false;

      float direction = final_force * this->_collition_normal;
      if (direction < 0) {
        float v_n = this->_velocity * this->_collition_normal;
        AngDir2 friction = (this->_collition_normal * v_n) - this->_velocity;

        if (friction.modulo2() > 0.0001)
          final_force = friction.normalize() * this->_acc_f_k * -direction;
        else
          final_force = AngDir2 {0.f, 0.f, 0.f};
      }
    }

    AngDir2 coef_mult(this->glb->get_time() * DRAW_RATE);

    final_force *= 20000.f / (this->_density * this->_area);
    this->_velocity += final_force.dir_mul (coef_mult);
    this->position += this->_velocity.dir_mul (coef_mult);

    this->reposition_polygon();
  }
}
    
template<std::size_t N>
void NEdge<N>::set_position (AngDir2 center) {
  this->position = center;
  this->reposition_polygon();
}
