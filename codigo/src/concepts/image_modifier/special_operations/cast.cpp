#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>

Visualizer<D2FIG> ImageModifier::cast (SDL_Renderer* render) {
  Visualizer<D2FIG> ret = Visualizer<D2FIG> ();
  ret.width = this->texture->w;
  ret.height = this->texture->h;
  ret.texture = SDL_CreateTextureFromSurface(render, this->texture);
  return ret;
}
