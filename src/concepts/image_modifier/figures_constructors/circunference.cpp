#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
   
ImageModifier ImageModifier::circunference (Arena& arena, int radio, int width, SDL_Color color) {
  ArenaConstexFlag context = arena.get_context();

  uint32_t dim = 2*radio;

  std::size_t r1 = radio - width;
  std::size_t b1 = 4*radio - 4;
  std::size_t b2 = 4*r1 - 4;
  int* bounds1 = arena.atalloc<int>(b1);
  int* bounds2 = arena.atalloc<int>(b2);
  int* bounds3 = arena.atalloc<int>(b2 + b1);
  circle_bound_maker (radio, bounds1, 0);
  circle_bound_maker (r1, bounds2, width);

  std::size_t i = 0, j = 0, k = 0;
  while (i < b1 && j < b2) {
    if (bounds1[i] < bounds2[j]) {
      bounds3[k] = bounds1[i++];
    } else if (bounds2[j] < bounds1[i]) {
      bounds3[k] = bounds2[j++];
    } else {
      bounds3[k++] = bounds1[i++];
      bounds3[k] = bounds2[j++];
    }
    k++;
  }
  while (i < b1)
    bounds3[k++] = bounds1[i++];

  bounds3[k] = dim * dim;
  ImageModifier ret = ImageModifier::bound_constructor(bounds3, dim, dim, color);
  
  arena.go_back_context(context);

  return ret;
}
