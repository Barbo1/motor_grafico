#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::operator& (const ImageModifier& img) {
  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, this->texture->w, this->texture->h, 
    32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );

  if (sur != nullptr) {
    Uint32* fin = (Uint32*)sur->pixels;
    Uint32* arr = (Uint32*)img.texture->pixels;

    int i = 0;
    for (; i < std::min(img.texture->h, this->texture->h); i++) {
      for (int j = 0; j < std::min(img.texture->w, this->texture->w); j++)
        fin[i * this->texture->w + j] = arr[i * img.texture->w + j];
    }

    arr = (Uint32*)this->texture->pixels;

    SDL_Color color;
    Uint8 a1, a2;
    for (int i = 0; i < this->texture->h; i++) {
      for (int j = 0; j < this->texture->w; j++) {
        SDL_GetRGBA (*fin, this->texture->format, &color.r, &color.g, &color.b, &a1);
        SDL_GetRGBA (arr[i * this->texture->w + j], this->texture->format, &color.r, &color.g, &color.b, &a2);
        if (a1 == 0 || a2 == 0)
          *fin = 0;
        fin++;
      }
    }

    SDL_FreeSurface(this->texture);
    this->texture = sur;
  }
  return *this;
}
