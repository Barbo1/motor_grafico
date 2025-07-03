#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/texture.hpp"
#include <cstdint>

class Square: public Physical, public Texture {
  private:
    int height;
    int width;

    Square ();

  public: 
    Square(
      SDL_Renderer*, uint32_t, uint32_t, AngularDirection, SDL_Color, 
      float = 0, float = 0, float = 0, float = 0, bool = true, bool = true
    );
    Square (uint32_t, uint32_t, Direction, SDL_Color);
    Square (const Square &);
    Square (Square &&);
    Square & operator= (const Square &);
    Square & operator= (Square &&);

    void draw (SDL_Renderer *);
};
