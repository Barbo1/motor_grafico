#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

template <VisualType T>
void Visualizer<T>::set_alpha (Uint8 a) {
  SDL_SetTextureBlendMode(this->texture.get(), SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(this->texture.get(), a);
}
