#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <vector>

#include "../primitives/vectors.hpp"

#define BUCKET_LINES_ESTIMATED_PARTITIONS 16

/***********************/
/*       Objects       */
/***********************/

struct Light {
  float intensity;
  float attenuation;
  Dir2 position;
  struct LightColorTy{
    float r, g, b;
  } color;
};

struct MaskObject {
  Dir2 point1;
  Dir2 point2;
  bool circle;
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
      const Dir2& position, const std::vector<MaskObject>& segments, const Uint32 shadow_color
    );
    ViewMask& draw_light_view_mask (
      const Light& light, const std::vector<MaskObject>& segments, const Uint32 shadow_color
    );
    ViewMask& draw_color_directional_mask (
      const Dir2& direction, const std::vector<MaskObject>& segments, const Uint32 shadow_color
    );
    ViewMask& draw_light_directional_mask (
      const Dir2& direction, const std::vector<MaskObject>& segments, const Uint32 shadow_color
    );

    /* mask fusion. */
    ViewMask operator& (ViewMask mask);
    ViewMask operator| (ViewMask mask);

    friend Global;

    ~ViewMask();
};


/**************************/
/*       Light Math       */
/**************************/

enum ViewCoveringType {point, direction};

/* This function is meant to draw a shadow in the buffer of a view mask. The 
 * function takes an array of 8, meaning that it receives 6 points denoting
 * the polygon.
 * */
void cast_shadow (Uint32*& buffer, int32_t width, int32_t height, const std::array<Dir2, 8>& points, Uint32 color);

/* This function uses cast_shadow to create the shadows for a view in the 
 * surface img. 
 * */
void fill_view_with_shadows (
  SDL_Surface*& img, 
  const Dir2& position, 
  const std::vector<MaskObject>& segments, 
  const Uint32 color
);

/* This function fill the remaining space of a view with light from the 
 * position of the light to all the places with no color defined (0). 
 * */
void fill_view_with_lights (
  SDL_Surface*& img, 
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
std::vector<MaskObject> generate_view_covering_by_point (const Dir2& position, const std::vector<MaskObject>& segments);
std::vector<MaskObject> generate_view_covering_by_direction (const Dir2& direction, const std::vector<MaskObject>& segments);
