#include "../../../../headers/primitives/global.hpp"
#include <SDL2/SDL_timer.h>

float Global::get_time () {
  return (SDL_GetPerformanceCounter() - this->clock_ticks) / (float)SDL_GetPerformanceFrequency();
}
