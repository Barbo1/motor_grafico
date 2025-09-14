#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
void Visualizer<T>::draw (SDL_Renderer* render, const AngDir2 & position) const {
  SDL_Rect dst;
  dst.x = static_cast<int>(position.x);
  dst.y = static_cast<int>(position.y); 
  dst.h = this->height;
  dst.w = this->width;
  SDL_Point center = {0, 0};
  SDL_SetTextureBlendMode (this->texture, SDL_BLENDMODE_BLEND);
  SDL_RenderCopyEx (render, this->texture, nullptr, &dst, position.a, &center, SDL_FLIP_NONE);
}

template void Visualizer<D2FIG>::draw (SDL_Renderer* render, const AngDir2 & position) const;
