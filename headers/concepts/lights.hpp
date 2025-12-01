#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <vector>

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"

#define BUCKET_LINES_ESTIMATED_PARTITIONS 4

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

struct MaskLineInfo {
  Dir2 point1;
  Dir2 point2;

  float center_distance;
  int32_t partition_offset;
};

struct FirstLevelElement {
  std::vector<MaskLineInfo> data;
  int32_t offset;
  int32_t second_level_offset;
  int32_t last_second_level_offset;
};

/* Generate a covering view over the lines passed in 'segments', meaning 
 * that it will return the lines that can be "viewed" from this position. 
 * */
std::vector<MaskObject> generate_view_covering (Dir2 position, const std::vector<MaskObject>& segments);

class ViewMask {
  private:
    SDL_Surface* img;

  public:
    /* Creation of a uniform mask. */
    ViewMask (Global glb, SDL_Color color);

    ViewMask create_view_mask_by_point (Global glb, Dir2 position, const std::vector<MaskObject>& segments);
    ViewMask create_light_screen_by_point(Global glb, Light light, const std::vector<MaskObject>& segments);
    ViewMask create_light_mask_by_point(Global glb, Light light, const std::vector<MaskObject>& segments);

    /* mask operations. */
    ViewMask operator& (ViewMask mask);
    ViewMask operator| (ViewMask mask);

    ~ViewMask();
};
