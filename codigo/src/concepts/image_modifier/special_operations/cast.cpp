#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>

Visualizer<D2FIG> ImageModifier::cast (Global* glb) {
  Visualizer<D2FIG> ret = Visualizer<D2FIG> ();
  ret.width = this->texture->w;
  ret.height = this->texture->h;
  ret.texture = std::shared_ptr<SDL_Texture>(
    SDL_CreateTextureFromSurface(glb->render, this->texture), 
    [] (SDL_Texture* texture) { SDL_DestroyTexture (texture); }
  );
  return ret;
}
