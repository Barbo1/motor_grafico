#include "../../../../headers/pr_objects/square.hpp"

Square::Square (
  SDL_Renderer* render, uint32_t height, uint32_t width, AngDir2 center, SDL_Color color, 
  float density, float f_s, float f_k, bool movible, bool colidable
) : 
  Physical (center, density, height * width, f_s, f_k, movible, colidable),
  texture (Visualizer<D2FIG>::square(render, 2*height, 2*width, color)),
  height(height),
  width(width)
{}
