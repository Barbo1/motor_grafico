#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>

#include "../primitives/vectors.hpp"

#define BUCKET_LINES_ESTIMATED_PARTITIONS 16

struct Light {
  Dir2 position;
  SDL_Color color;
  float intensity;
};

/* Objects to help construct the arrange of buckets to discard lines. */

struct MaskObject {
  Dir2 point1;
  Dir2 point2;
  bool circle;
};

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
 * it will return the ones that can be "viewed" from the position. 
 * */
std::vector<MaskObject> generate_view_covering_by_point (Dir2 position, const std::vector<MaskObject>& segments);

/* Generate a covering view over the segments due a direction. The main difference 
 * between the position's and direction's versions is that this one make a covering 
 * resembling a parallel view.
 * */
std::vector<MaskObject> generate_view_covering_by_direction (Dir2 direction, const std::vector<MaskObject>& segments);

class Global;

class ViewMask {
  private:
    SDL_Surface* img;

    ViewMask (SDL_Surface*);

  public:
    /* Creation of a uniform mask. */
    ViewMask (uint32_t width, uint32_t height, SDL_Color color);
    ViewMask (uint32_t width, uint32_t height);

    static ViewMask create_view_mask_by_direction (
      const uint32_t& width, const uint32_t& height, 
      Dir2 direction, const std::vector<MaskObject>& segments,
      const Uint32 color
    );
    static ViewMask create_view_mask_by_point (
      const uint32_t& width, const uint32_t& height, 
      Dir2 position, const std::vector<MaskObject>& segments,
      const Uint32 color
    );
    static ViewMask create_light_screen_by_point (
      const uint32_t& width, const uint32_t& height, 
      Light light, const std::vector<MaskObject>& segments,
      const Uint32 color
    );
    static ViewMask create_light_mask_by_point (
      const uint32_t& width, const uint32_t& height, 
      Light light, const std::vector<MaskObject>& segments,
      const Uint32 color
    );

    /* mask drawing. */
    ViewMask& draw_view_mask_by_direction (
      const Dir2& direction, const std::vector<MaskObject>& segments, const Uint32 color
    );
    ViewMask& draw_view_mask_by_point (
      const Dir2& position, const std::vector<MaskObject>& segments, const Uint32 color
    );
    ViewMask& draw_light_screen_by_point (
      const Light& light, const std::vector<MaskObject>& segments, const Uint32 color
    );
    ViewMask& draw_light_mask_by_point (
      const Light& light, const std::vector<MaskObject>& segments, const Uint32 color
    );

    /* mask operations. */
    ViewMask operator& (ViewMask mask);
    ViewMask operator| (ViewMask mask);

    friend Global;

    ~ViewMask();
};
