#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <iostream>

void Visualizer<D3FIG>::set_texture (const std::string& img) {
  if (std::holds_alternative<SDL_Surface*>(this->texture))
    SDL_FreeSurface(std::get<SDL_Surface*>(this->texture));

  if (img != "") {
    int w, h;
    Uint32* pixels = charging_PNG_to_memory (img, w, h);
    if (pixels != nullptr) {
      SDL_Surface * readed_img = SDL_CreateRGBSurfaceFrom (
        pixels, w, h, 32, w * 4, 
        0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
      );
      if (readed_img == nullptr) {
        std::cout 
          << "Error while reading " << img
          << "error found while reading the image." << std::endl;
        delete [] pixels;
        this->texture = SDL_Color {.r = 255, .g = 0, .b = 255, .a = 255};
      } else this->texture = readed_img;
    }
  } else std::cout << "Error while reading '" << img << "' image path not provided." << std::endl;
}

