#include "../../../../../headers/concepts/texture.hpp"
#include <SDL2/SDL_surface.h>
#include <variant>

Visualizer<D3FIG>::~Visualizer () {
  (*this->use_count)--;
  if (*this->use_count <= 0) {
    delete this->info;
    delete this->use_count;
    if (std::holds_alternative<SDL_Surface*>(this->texture))
      SDL_FreeSurface(std::get<SDL_Surface*>(this->texture));
  }
}

