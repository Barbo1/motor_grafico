#include "../../../../headers/primitives/global.hpp"

void Global::apply_mask (const ViewMask& mask) {
  SDL_UpdateTexture (this->render_mask, NULL, mask.img->pixels, mask.img->pitch);
  SDL_Rect dst;
  dst.x = 0;
  dst.y = 0; 
  dst.w = this->c_width;
  dst.h = this->c_height;
  SDL_RenderCopy (this->render, this->render_mask, nullptr, &dst);
}
