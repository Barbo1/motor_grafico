#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
   
ImageModifier ImageModifier::oval (Arena& arena, int height, int base, SDL_Color color) {
  ArenaConstexFlag context = arena.get_context();

  int* bounds = arena.atalloc<int>(4*height- 4);
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

  ImageModifier ret = ImageModifier::bound_constructor(bounds, 2*height, 2*base, color);
  arena.go_back_context(context);
  return ret;
}
