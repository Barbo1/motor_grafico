#include "../../../../headers/primitives/global.hpp"
#include <SDL2/SDL_timer.h>

float Global::get_time () {
  return (SDL_GetTicks() - this->clock_ticks) / 1000.f;
}
