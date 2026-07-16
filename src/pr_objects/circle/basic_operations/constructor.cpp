#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/primitives/global.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

Circle::Circle (
  Global* glb, Arena& arena, uint32_t radio, AngDir2 center, float density, 
  float f_k, bool movible, SDL_Color* color
) noexcept : 
  Physical (glb, center, density, M_PI * radio * radio, 0.f, f_k, movible), 
  radio (radio)
{
  this->glb = glb;
  float mass = density * (M_PI * radio * radio);
  this->inertia = 0.5f * mass * radio * radio;
  if (color != nullptr) {
    this->texture = ImageModifier::circle(arena, radio, *color).cast(this->glb);
  }
}
