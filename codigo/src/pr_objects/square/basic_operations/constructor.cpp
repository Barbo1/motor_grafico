#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

Square::Square (
  Global* glb, uint32_t height, uint32_t width, AngDir2 center, 
  float density, float f_k, bool movible, bool colidable, SDL_Color* color
) : 
  Physical (glb, center, density, height * width, f_k, movible, colidable),
  width(width),
  height(height)
{
  this->glb = glb;
  if (color != nullptr) {
    this->texture = ImageModifier::square(2*height, 2*width, *color).cast(this->glb);
  }
}
