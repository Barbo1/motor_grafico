#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

template <VisualType T>
void Visualizer<T>::draw (Global* glb, const AngDir2 & position) const {
  SDL_Rect dst;
  dst.x = static_cast<int>(position.x) - this->width / 2;
  dst.y = static_cast<int>(position.y) - this->height / 2; 
  dst.w = this->width;
  dst.h = this->height;
  SDL_SetTextureBlendMode (this->texture.get(), SDL_BLENDMODE_BLEND);
  SDL_RenderCopyEx (glb->get_render(), this->texture.get(), nullptr, &dst, position.a, nullptr, SDL_FLIP_NONE);
}

template void Visualizer<D2FIG>::draw (Global* glb, const AngDir2 & position) const;
