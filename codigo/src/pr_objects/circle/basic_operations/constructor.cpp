#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

Circle::Circle (
  SDL_Renderer* render, uint32_t radio, AngDir2 center, float density, 
  float f_k, bool movible, bool colidable, SDL_Color* color
) : 
  Physical (center, density, M_PI * radio * radio, f_k, movible, colidable), 
  radio (radio)
{
  if (color != nullptr) {
    this->texture = ImageModifier::circle(radio, *color).cast(render);
  }
}
