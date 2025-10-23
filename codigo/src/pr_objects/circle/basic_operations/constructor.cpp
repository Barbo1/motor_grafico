#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/primitives/global.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

Circle::Circle (
  Global* glb, uint32_t radio, AngDir2 center, float density, 
  float f_k, bool movible, bool colidable, SDL_Color* color
) : 
  Physical (glb, center, density, M_PI * radio * radio, f_k, movible, colidable), 
  radio (radio)
{
  this->glb = glb;
  if (color != nullptr) {
    this->texture = ImageModifier::circle(radio, *color).cast(this->glb->get_render());
  }
}
