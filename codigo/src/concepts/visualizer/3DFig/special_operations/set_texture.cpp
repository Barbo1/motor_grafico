#include "../../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

void Visualizer<D3FIG>::set_texture (const ImageModifier& img) {
  if (std::holds_alternative<SDL_Surface*>(this->texture))
    SDL_FreeSurface(std::get<SDL_Surface*>(this->texture));
  this->texture = SDL_ConvertSurface(img.texture, img.texture->format, 0);
}

