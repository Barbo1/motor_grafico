#include "../../../../headers/pr_objects/nedge.hpp"

template<std::size_t N>
void NEdge<N>::set_texture (Visualizer<D2FIG> tex) {
  this->texture = tex;
}
