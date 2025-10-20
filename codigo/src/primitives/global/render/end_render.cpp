#include "../../../../headers/primitives/global.hpp"
#include <SDL2/SDL_timer.h>

void Global::end_render () {
  this->clock_ticks = SDL_GetPerformanceCounter();
  SDL_SetRenderTarget (this->render, nullptr);
  SDL_RenderCopy (this->render, this->global_render_texture, nullptr, nullptr);
  SDL_RenderPresent (this->render);
}
