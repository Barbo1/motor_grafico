#include "./types_definition.hpp"
#include "./bool_matrix.hpp"
#include "./vectors.hpp"

#include <vector>
#include <SDL2/SDL_render.h>

SDL_Surface* raster_grade_1 (std::vector<Dir2>, SDL_Color, AntiAliasingType);

SDL_Surface* raster_grade_2 (std::vector<Dir2>, std::vector<bool>, SDL_Color, AntiAliasingType);
