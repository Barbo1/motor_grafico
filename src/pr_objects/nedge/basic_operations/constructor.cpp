#include "../../../../headers/pr_objects/nedge.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

template<std::size_t N>
NEdge<N>::NEdge (Global* glb, const std::vector<Dir2> & points, SDL_Color* color, int* error) noexcept {
  if (points.size() < N) {
    if (error != nullptr)
      *error = -1;
    return;
  }

  this->glb = glb;

  // writing points.
  for (uint32_t i = 0; i < N; i++)
    this->points[i] = points[i];

  // defining triangles.
  /*
  Dir2 mult = Dir2(-1.f, 1.f);
  Dir2 aux = points[N-1].dir_mul(mult) + points[0];
  float order = aux.y * aux.x;
  for (uint32_t i = 0; i < N-1; i++) {
    aux = points[i].dir_mul(mult) + points[i+1];
    order += aux.y * aux.x;
  }
  bool clockwise = order > 0.f;
  */


  if (color != nullptr) {
    this->texture = ImageModifier::polygon(points, *color).cast(glb);
  }
}
