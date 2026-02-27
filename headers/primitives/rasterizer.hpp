#include "./types_definition.hpp"
#include "./bool_matrix.hpp"
#include "./vectors.hpp"

#include <vector>
#include <SDL2/SDL_render.h>

SDL_Surface* raster_grade_1 (
  std::vector<Dir2>, 
  SDL_Color, 
  AntiAliasingType
);

enum ComponentElementType { LINE, CURVE };

struct ComponentElement {
  Dir2 start, middle, end;
  ComponentElementType t;
};

SDL_Surface* raster_grade_2 (
  const std::vector<std::vector<ComponentElement>>&, 
  const Dir2&, 
  const Dir2&, 
  SDL_Color, 
  AntiAliasingType
);
