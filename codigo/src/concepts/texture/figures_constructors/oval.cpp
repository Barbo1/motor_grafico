#include "../../../../headers/concepts/texture.hpp"

Texture Texture::oval (SDL_Renderer* render, int height, int base, SDL_Color color) {
  int* bounds = new int[4*height- 4];
  int* biter = bounds;

  float a_b = static_cast<float>(base) / static_cast<float>(height), b_2 = height * height;
  float y = height-1;
  int x = static_cast<int>(a_b*sqrt(b_2 - y * y)); 
  int level = 0;
  for (; y > 0; y-=1, x = static_cast<int>(a_b*sqrt(b_2 - y * y))) {
    *(biter++) = base-x;
    *(biter++) = base+x;
    level++;
  }
  for (int* rbiter = biter-1; rbiter >= bounds; rbiter-=2) {
    *(biter++) = *(rbiter-1);
    *(biter++) = *rbiter;
    level++;
  }
  biter = bounds;
  for (int i = 0, sum = 0; i < level; i++, sum += 2*base) {
    *(biter++) += sum;
    *(biter++) += sum;
  }

  Texture ret = bounder(render, bounds, 2*height, 2*base, color, {base, height});
  delete [] bounds;
  return ret;
}

