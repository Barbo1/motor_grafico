#include "../../../../headers/concepts/image_modifier.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include "../../../../headers/primitives/rasterizer.hpp"
   
ImageModifier ImageModifier::polygon (Arena& arena, const Dir2* points, std::size_t size, SDL_Color color) {
  ImageModifier ret;
  ret.texture = raster_grade_1(points, size, color, AAx1, arena);
  ret.aquired = 0;
  return ret;
}
