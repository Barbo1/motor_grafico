#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

ImageModifier ImageModifier::circle (int radio, SDL_Color color) {
  uint32_t dim = 2*radio;
  Uint32* pixels = new Uint32[dim * dim];

  int* bounds = new int[4*radio - 4];
  circle_bound_maker (radio, bounds, 0);

  ImageModifier ret = ImageModifier::bound_constructor(bounds, pixels, dim, dim, color);
  delete [] bounds;

  return ret;
}
