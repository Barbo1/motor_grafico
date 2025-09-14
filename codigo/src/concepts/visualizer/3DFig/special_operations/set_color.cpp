#include "../../../../../headers/concepts/visualizer.hpp"

void Visualizer<D3FIG>::set_color (const SDL_Color& color) {
  if (std::holds_alternative<SDL_Surface*>(this->texture))
    SDL_FreeSurface(std::get<SDL_Surface*>(this->texture));

  this->texture = color;
}
