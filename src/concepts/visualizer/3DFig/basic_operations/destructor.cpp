#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_surface.h>
#include <variant>

Visualizer<D3FIG>::~Visualizer () {
  if (this->info.use_count() == 1)
    if (std::holds_alternative<SDL_Surface*>(this->texture))
      SDL_FreeSurface(std::get<SDL_Surface*>(this->texture));
}

