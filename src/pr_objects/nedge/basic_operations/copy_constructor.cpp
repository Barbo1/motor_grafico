#include "../../../../headers/pr_objects/nedge.hpp"

template<std::size_t N>
NEdge<N>::NEdge (const NEdge & poly) noexcept {
  this->points = poly.points;
  this->texture = poly.texture;
}
