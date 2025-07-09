#include "../../../../headers/concepts/texture.hpp"
#include "../utils/circle_bound_maker.cpp"
   
Texture Texture::circunference (SDL_Renderer* render, int radio, int width, SDL_Color color) {
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
  delete [] bounds1;
  delete [] bounds2;

  bounds3[k] = 4*radio*radio;

  Texture ret = bounder(render, bounds3, 2*radio, 2*radio, color, {radio, radio});
  delete [] bounds3;
  return ret;
}
