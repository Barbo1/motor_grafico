#include "../../../../headers/pr_objects/nedge.hpp"
#include <utility>

NEdge::NEdge (NEdge && poly) {
  this->texture = std::exchange (poly.texture, Visualizer<D2FIG>());
  this->points = std::exchange (poly.points, std::vector<Dir2>());
}
