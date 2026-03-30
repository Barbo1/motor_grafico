#include "../../../../headers/concepts/image_modifier.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include "../../../../headers/primitives/rasterizer.hpp"
#include <cstdint>
   
ImageModifier ImageModifier::polygon (const Dir2* points, std::size_t size, SDL_Color color) {
  Dir2 min = points[0];
  for (uint32_t i = 0; i < size; i++) {
    Dir2 point = points[i];
    min.v = _mm_min_ps (min.v, point.v);
  }
  ImageModifier ret;
  ret.texture = raster_grade_1(points, size, color, AAx1);
  ret.aquired = 0;
  return ret;
}
