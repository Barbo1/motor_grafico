#include "../../../../headers/primitives/global.hpp"
#include "../../../../headers/primitives/rasterizer.hpp"
#include <SDL2/SDL_surface.h>
#include <cmath>

void print_polygon_c (Global* glb, std::vector<Dir2> points, SDL_Color color) {
  Dir2 min = points[0];
  for (const auto& point: points)
    min.v = _mm_min_ps (min.v, point.v);
  SDL_Surface* surface = raster_grade_1(points, color, AAx8);
  SDL_Texture* tex = SDL_CreateTextureFromSurface(glb->get_render(), surface);
  SDL_Rect dst;
  dst.x = std::round(min.x);
  dst.y = std::round(min.y);
  dst.w = surface->w;
  dst.h = surface->h;
  SDL_RenderCopyEx (glb->get_render(), tex, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
  SDL_DestroyTexture(tex);
  SDL_FreeSurface (surface);
}
