#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier ImageModifier::rounded_square (int height, int base, int radio, SDL_Color color) {
  if (height < 2 * radio || base < 2 * radio)
    return ImageModifier(0, 0, nullptr);

  Uint32* pixels = new Uint32[height * base];

  int* bounds = new int[4*radio - 4];
  circle_bound_maker (radio, bounds, 0);
  int xdev = base - 2 * radio, 
    ydev = (height - 2 * radio) * base, 
    n = 1;
  bounds[1] += xdev;
  for (int i = 2; i < 4 * (radio - 1); i+=2) {
    bounds[i] += n++ * xdev;
    bounds[i+1] += n * xdev;
  }
  for (int i = 2 * (radio - 1); i < 4 * (radio - 1); i++)
    bounds[i] += ydev;
  bounds[2 * radio - 3] = bounds[2 * radio - 2];

  ImageModifier ret = ImageModifier::bound_constructor(bounds, pixels, height, base, color);
  delete [] bounds;

  return ret;
}
