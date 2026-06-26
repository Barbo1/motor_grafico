#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

Square::Square (
  Global* glb, uint32_t height, uint32_t width, AngDir2 center, 
  float density, float f_k, bool movible, SDL_Color* color
) noexcept : 
  Physical (glb, center, density, 4.f * height * width, 0.f, f_k, movible)
{
  Dir2 dims2 = Dir2(width, height);
  this->dims.store(dims2);
  this->glb = glb;
  this->inertia = dims2.modulo2();
  if (color != nullptr) {
    this->texture = ImageModifier::square(2*height, 2*width, *color).cast(this->glb);
  }
}
