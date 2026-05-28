#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
   
ImageModifier ImageModifier::circunference (int radio, int width, SDL_Color color) {
  uint32_t dim = 2*radio;
  Uint32* pixels = new Uint32[dim * dim];

  std::size_t r1 = radio - width;
  std::size_t b1 = 4*radio - 4;
  std::size_t b2 = 4*r1 - 4;
  int* bounds1 = new int[b1];
  int* bounds2 = new int[b2];
  int* bounds3 = new int[b1 + b2];
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
  ImageModifier ret = ImageModifier::bound_constructor(bounds3, pixels, dim, dim, color);

  delete [] bounds1;
  delete [] bounds2;
  delete [] bounds3;
  return ret;
}
