#include "../../../../headers/primitives/global.hpp"

void Global::begin_render () {
  this->clock_ticks = SDL_GetTicks();
  SDL_SetRenderTarget (this->render, this->global_render_texture);
  SDL_SetRenderDrawColor (
    this->render, this->c_bg_color.r, this->c_bg_color.g, 
    this->c_bg_color.b, this->c_bg_color.a
  );
  SDL_RenderClear (this->render);
}
