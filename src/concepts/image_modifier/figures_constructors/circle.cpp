#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>

ImageModifier ImageModifier::circle (Arena& arena, int radio, SDL_Color color) {
  ArenaConstexFlag context = arena.get_context();

  uint32_t dim = 2*radio;
  int* bounds = arena.atalloc<int>(4*radio - 4);
  circle_bound_maker (radio, bounds, 0);
  ImageModifier ret = ImageModifier::bound_constructor(bounds, dim, dim, color);

  arena.go_back_context(context);
  return ret;
}
