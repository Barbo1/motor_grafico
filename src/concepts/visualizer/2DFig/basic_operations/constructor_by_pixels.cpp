#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_render.h>

template<>
Visualizer<D2FIG>::Visualizer (Global* glb, int height, int width, Uint32* pixels) noexcept {
  this->width = width;
  this->height = height;
  SDL_Surface* sur = SDL_CreateRGBSurfaceFrom (
    pixels, width, height, 32, width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );
  this->texture = std::shared_ptr<SDL_Texture>(
    SDL_CreateTextureFromSurface(glb->get_render(), sur), 
    [] (SDL_Texture* texture) { SDL_DestroyTexture (texture); }
  );
  SDL_FreeSurface(sur);
}
