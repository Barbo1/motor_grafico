#include "../../../../headers/concepts/texture.hpp"
#include "../utils/circle_bound_maker.cpp"

Texture Texture::circle (SDL_Renderer* render, int radio, SDL_Color color) {
  int* bounds = new int[4*radio - 4];
  circle_bound_maker (radio, bounds, 0);

  Texture ret = bounder(render, bounds, 2*radio, 2*radio, color, {radio, radio});
  delete [] bounds;
  return ret;
}
