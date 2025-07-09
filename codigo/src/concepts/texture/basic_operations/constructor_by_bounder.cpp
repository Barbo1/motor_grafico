#include "../../../../headers/concepts/texture.hpp"

Texture::Texture(SDL_Renderer* render, int height, int width, SDL_Point & center, Uint32* pixels) {
  SDL_Surface* sur = SDL_CreateRGBSurfaceFrom (
    pixels, width, height, 32, width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );
  this->texture = SDL_CreateTextureFromSurface(render, sur);
  SDL_FreeSurface(sur);
  this->use_count = new int(1);
  this->width = width;
  this->height = height;
  this->center = center;
}
