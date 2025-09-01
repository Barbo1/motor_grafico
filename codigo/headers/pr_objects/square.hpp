#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/texture.hpp"
#include <cstdint>

class Square: public Physical {
  private:
    int height;
    int width;

    Visualizer<D2FIG> texture;

    Square ();

  public: 
    Square(
      SDL_Renderer*, uint32_t, uint32_t, AngDir2, SDL_Color, 
      float = 0, float = 0, float = 0, float = 0, bool = true, bool = true
    );
    Square (uint32_t, uint32_t, Dir2, SDL_Color);
    Square (const Square &);
    Square (Square &&);
    Square & operator= (const Square &);
    Square & operator= (Square &&);

    void draw (SDL_Renderer *);
};
