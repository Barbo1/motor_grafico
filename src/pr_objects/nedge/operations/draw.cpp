#include "../../../../headers/pr_objects/nedge.hpp"

template<std::size_t N>
void NEdge<N>::draw () {
  this->texture.draw(glb, this->position);
}
