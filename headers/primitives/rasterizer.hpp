#include "./types_definition.hpp"
#include "./bool_matrix.hpp"
#include "./vectors.hpp"

#include <vector>
#include <SDL2/SDL_render.h>

SDL_Surface* raster_lines (std::vector<Dir2>, SDL_Color, AntiAliasingType);
