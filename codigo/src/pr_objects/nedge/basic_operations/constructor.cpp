#include "../../../../headers/pr_objects/nedge.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

NEdge::NEdge (Global* glb, const std::vector<Dir2> & points, SDL_Color* color) {
  glb = glb;
  this->points = points;
  if (color != nullptr) {
    this->texture = ImageModifier::polygon(points, *color).cast(glb->get_render());
  }
}
