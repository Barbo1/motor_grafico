#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::resize (uint32_t height, uint32_t width) {
  if (this->texture == nullptr)
    return *this;

  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, height, width, 32,
    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );
  if (sur != nullptr) {
    if (SDL_BlitScaled (this->texture, nullptr, sur, nullptr)) {
      return *this;
      SDL_FreeSurface(this->texture);
    }

    if (this->aquired)
      delete [] (Uint32*)this->texture->pixels;
    SDL_FreeSurface(this->texture);

    this->texture = sur;
    this->aquired = 0;
  }
  return *this;
}
