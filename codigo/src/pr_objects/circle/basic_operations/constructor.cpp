#include "../../../../headers/pr_objects/circle.hpp"

Circle::Circle (
  SDL_Renderer* render, uint32_t radio, AngDir2 center, SDL_Color color, float density,
  float f_s, float f_k, bool movible, bool colidable
) : 
  Physical (center, density, M_PI * radio * radio, f_s, f_k, movible, colidable), 
  radio (radio),
  texture(Visualizer<D2FIG>::circle(render, radio, color))
{}
