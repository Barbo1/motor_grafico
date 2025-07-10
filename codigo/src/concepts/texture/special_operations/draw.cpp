#include "../../../../headers/concepts/texture.hpp"

void Texture::draw (SDL_Renderer* render, const AngDir2 & position) const {
  SDL_Rect dst;
  dst.x = static_cast<int>(position.x);
  dst.y = static_cast<int>(position.y); 
  dst.h = this->height;
  dst.w = this->width;
  SDL_SetTextureBlendMode (this->texture, SDL_BLENDMODE_BLEND);
  SDL_RenderCopyEx (render, this->texture, nullptr, &dst, position.a, &this->center, SDL_FLIP_NONE);
}
