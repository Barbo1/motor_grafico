#include "../../../../headers/pr_objects/nedge.hpp"

NEdge::NEdge (SDL_Renderer* render, const std::vector<Dir2> & points, SDL_Color color) {
  this->points = points;
  this->texture = Visualizer<D2FIG>::polygon(render, points, color);
}
