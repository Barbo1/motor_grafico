#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_render.h>

template<>
Visualizer<D2FIG>::Visualizer (Global* glb, int height, int width, SDL_Color color) noexcept {
  this->height = height;
  this->width = width;
  this->texture = std::shared_ptr<SDL_Texture>(
    SDL_CreateTexture(glb->get_render(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height), 
    [] (SDL_Texture* texture) { SDL_DestroyTexture (texture); }
  );
  
  SDL_Texture* actual_texture = SDL_GetRenderTarget(glb->get_render());
  SDL_SetRenderTarget(glb->get_render(), this->texture.get());
    SDL_SetRenderDrawColor(glb->get_render(), color.r, color.g, color.b, color.a);
    SDL_RenderClear(glb->get_render());
  SDL_SetRenderTarget(glb->get_render(), actual_texture);
}
