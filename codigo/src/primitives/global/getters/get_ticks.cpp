#include "../../../../headers/primitives/global.hpp"
#include <SDL2/SDL_timer.h>

uint32_t Global::get_ticks () {
  return SDL_GetTicks() - this->clock_ticks;
}
