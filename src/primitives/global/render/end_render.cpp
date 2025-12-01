#include "../../../../headers/primitives/global.hpp"
#include <SDL2/SDL_timer.h>

void Global::end_render () {
  SDL_SetRenderTarget (this->render, nullptr);
  SDL_RenderCopy (this->render, this->global_render_texture, nullptr, nullptr);
  SDL_RenderPresent (this->render);
}

/*
  SDL_SetRenderDrawBlendMode (this->render, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor (
    this->render, 
    this->darkness_color.r, this->darkness_color.g, this->darkness_color.b, this->darkness_color.a
  );
  SDL_Rect screen = {0, 0, (int)this->c_width, (int)this->c_height};
  SDL_RenderFillRect (this->render, &screen);
 * */
