#include "../../../../headers/primitives/global.hpp"

void Global::apply_mask (const ViewMask& mask) {
  SDL_Texture* img = SDL_CreateTextureFromSurface(this->render, mask.img);
  SDL_Rect dst;
  dst.x = 0;
  dst.y = 0; 
  dst.w = this->c_width;
  dst.h = this->c_height;
  SDL_RenderCopy (this->render, img, nullptr, &dst);
  SDL_DestroyTexture (img);
}
