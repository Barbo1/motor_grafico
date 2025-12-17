#include "../../../../headers/primitives/global.hpp"
#include <cstring>

Global::~Global () {
  SDL_DestroyRenderer (Global::single->render);
  SDL_DestroyWindow (Global::single->window);
  SDL_DestroyTexture (Global::single->global_render_texture);
  SDL_DestroyTexture (Global::single->render_mask);
  SDL_Quit ();
  delete Global::single;
}
