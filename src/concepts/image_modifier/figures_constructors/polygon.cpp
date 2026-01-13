#include "../../../../headers/concepts/image_modifier.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include "../../../../headers/primitives/rasterizer.hpp"
   
ImageModifier ImageModifier::polygon (std::vector<Dir2> points, SDL_Color color) {
  Dir2 min = points[0];
  for (const auto& point: points)
    min.v = _mm_min_ps (min.v, point.v);
  ImageModifier ret;
  ret.texture = raster_grade_1(points, color, AAx1);
  ret.aquired = 0;
  return ret;
}
