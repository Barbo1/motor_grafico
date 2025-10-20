#include "../../../../headers/primitives/global.hpp"
#include <cstring>

Global::~Global () {
  SDL_DestroyRenderer (Global::single->render);
  SDL_DestroyWindow (Global::single->window);
  SDL_Quit ();
  delete Global::single;
}
