#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>

ImageModifier ImageModifier::subimage(Dir2 pos, Dir2 dim) {
  int width = dim.x;
  int height = dim.y;
  Uint32* arr = new Uint32[width * height];
  ImageModifier ret(height, width, arr);
  if (this->texture != nullptr) {
    int posx = pos.x;
    int posy = pos.y;
    uint32_t curr_width = std::min(width, this->texture->w - posx);
    uint32_t curr_height = std::min(height, this->texture->h - posy);
    for (uint32_t h = 0; h < curr_height; h++) {
      memcpy(
        arr + h*width, 
        static_cast<Uint32*>(this->texture->pixels) + (posy + h)*this->texture->w + posx, 
        sizeof(Uint32) * curr_width
      );
    }
  }
  return ret;
}
