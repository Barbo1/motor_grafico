#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <span>

#include "../primitives/types_definition.hpp"
#include "../primitives/vectors.hpp"
#include "../primitives/arena.hpp"
#include "../primitives/math.hpp"

#define BUCKET_LINES_ESTIMATED_PARTITIONS 16

/***********************/
/*       Objects       */
/***********************/

struct Light {
  float intensity;
  float attenuation;
  Dir2 position;
  struct LightColorTy {
    float r, g, b;
  } color;
};

struct MaskObjectList {
  MaskObject* obj;
  std::size_t size;
};

class Global;
class ViewMask {
  private:
    SDL_Surface* img;

    ViewMask (SDL_Surface*);

  public:
    /* Creation of a uniform mask. */
    ViewMask (uint32_t width, uint32_t height);

    /* mask drawing. */
    ViewMask& draw_color_uniform_mask (const Uint32 color);

    ViewMask& draw_light_uniform_mask (const Light& light);

    ViewMask& draw_color_view_mask (
      DynamicalArena& darena, 
      const MaskObjectList& segments,
      const Dir2& position
    );

    ViewMask& draw_color_directional_mask (
      DynamicalArena& darena, 
      const MaskObjectList& segments,
      const Dir2& direction
    );
    
    ViewMask& draw_light_view_mask (
      DynamicalArena& darena,
      const MaskObjectList& segments,
      const Light& light, 
      const Dir2& screen_dims 
    );

    ViewMask& draw_light_view_mask (
      Arena& arena,
      DynamicalArena& darena,
      const std::span<std::pair<Light, MaskObjectList>, std::dynamic_extent>& lights,
      const Dir2& screen_dims 
    );

    /* mask fusion. */
    ViewMask& operator| (const ViewMask&);

    friend Global;

    ~ViewMask();
};


/**************************/
/*       Light Math       */
/**************************/

/* This function is meant to draw a shadow in the buffer of a view mask. The 
 * function takes an array of 8, meaning that it receives 6 points denoting
 * the polygon.
 * */
void cast_shadow (
  Uint32*& buffer, 
  int32_t width, 
  int32_t height, 
  const std::array<Dir2, 8>& points,
  uint32_t many_points
);

/* This function uses cast_shadow to create the shadows for a view in the 
 * surface img. 
 * */
void fill_view_with_shadows (
  Uint32*& buffer, 
  int32_t width, 
  int32_t height, 
  DynamicalArena& darena,
  const MaskObjectList& segments,
  const Dir2& position
);

/* This function uses cast_shadow to create the shadows for a directional 
 * view in the surface img. 
 * */
void fill_directional_with_shadows (
  Uint32*& buffer, 
  int32_t width, 
  int32_t height, 
  DynamicalArena& darena,
  const MaskObjectList& segments,
  const Dir2& direction
);

/* This function fill the remaining space of a view with light from the 
 * position of the light to all the places with no color defined (0). 
 * */
void fill_remain_with_lights (
  Uint32*& buffer, 
  int32_t width, 
  int32_t height, 
  const Light& light
);

/* Objects to help construct the arrange of buckets to discard lines. */

struct SecondLevelElement {
  Dir2 point1;
  Dir2 point2;
  int32_t partition_offset;
};

struct FirstLevelElement {
  std::vector<SecondLevelElement> data;
  int32_t first_level_offset;
  int32_t first_second_level_offset;
  int32_t last_second_level_offset;
};

/* Generate a covering view over the segments passed in 'segments', meaning that 
 * it will return the ones that can be "viewed" from the position or through a 
 * direction. The main difference between the position's and direction's versions 
 * is that this one make a covering resembling a parallel view.
 * */
enum ViewGeneration {
  POINT,
  DIRECTION
};

MaskObjectList generate_view_covering (
  DynamicalArena& darena,
  const MaskObjectList& segments, 
  const Dir2& position, 
  ViewGeneration by_what
);

MaskObjectList filter_lines_point_view (
  DynamicalArena& darena,
  const MaskObjectList& segments,
  const Light& light, 
  const Dir2 screen_dims
);

/* This function returns the center and dimension of the lighted zone of a 
 * light. The calculations depends if the light is inside the screen or not.
 *
 * 'light' is the light which bounds are needed.
 * 'screen_dims' is the dimension of the complete screen (not half of it),
 * */
inline std::array<Dir2, 2> find_light_bounding(const Light& light, const Dir2& screen_dims) {
  __m128 half = _mm_set1_ps(0.5f);
  __m128 aten = _mm_set1_ps(light.attenuation);
  __m128 light_pos = Dir2(light.position).v;
  __m128 light_dims = _mm_sqrt_ps(_mm_mul_ps(
    _mm_fmadd_ps(_mm_set1_ps(light.intensity), half, _mm_set1_ps(-1.f)),
    _mm_rcp_ps(_mm_mul_ps(aten, aten))
  ));

  __m128 b = _mm_add_ps(light_pos, light_dims);
  __m128 d = _mm_sub_ps(light_pos, light_dims);
  __m128 min = _mm_min_ps(screen_dims.v, b);
  __m128 max = _mm_max_ps(_mm_setzero_ps(), d);

  Dir2 K = Dir2::from_well(_mm_mul_ps(_mm_add_ps(min, max), half));
  Dir2 dimsK = Dir2::from_well(_mm_sub_ps(min, K.v));

  Dir2 screen_pos = Dir2::from_well(_mm_mul_ps(screen_dims.v, half));
  if (!test_point_inside_square (light_pos, screen_pos, screen_pos)) {
    __m128 aux = _mm_sub_ps(K.v, light_pos);
    __m128 aux_abs = _mm_andnot_ps(_mm_set1_ps(-0.f), aux);
    __m128 light_dims_2 = _mm_mul_ps(light_dims, half);
    __m128 u = _mm_and_ps (
      _mm_or_ps (light_dims_2, _mm_and_ps(_mm_set1_ps(-0.f), aux)),
      _mm_cmplt_ps (_mm_permute_ps(aux_abs, 0b00010001), aux_abs)
    );
    K = Dir2::from_well(_mm_add_ps(light_pos, u));
    dimsK = Dir2::from_well(_mm_sub_ps(light_dims, _mm_andnot_ps(_mm_set1_ps(-0.f), u)));
  }

  return {K, dimsK};
}
