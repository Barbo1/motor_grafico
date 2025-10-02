#include "../../../../headers/pr_objects/nedge.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"

NEdge::NEdge (SDL_Renderer* render, const std::vector<Dir2> & points, SDL_Color* color) {
  this->points = points;
  if (color != nullptr) {
    this->texture = ImageModifier::polygon(points, *color).cast(render);
  }
}
