#include "../../../../headers/pr_objects/nedge.hpp"
#include <utility>

template<std::size_t N>
NEdge<N> & NEdge<N>::operator= (NEdge && poly) noexcept {
  this->texture = std::exchange (poly.texture, Visualizer<D2FIG>());
  this->points = std::exchange (poly.points, std::vector<Dir2>());
  return *this;
}
