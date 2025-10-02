#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

Square::Square (
  SDL_Renderer* render, uint32_t height, uint32_t width, AngDir2 center, 
  float density, float f_s, float f_k, bool movible, bool colidable, 
  SDL_Color* color
) : 
  Physical (center, density, height * width, f_s, f_k, movible, colidable),
  height(height),
  width(width)
{
  if (color != nullptr) {
    this->texture = ImageModifier::square(2*height, 2*width, *color).cast(render);
  }
}
