#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/texture.hpp"

class Square: public Texture, public Physical {
  private:
    int height;
    int width;

    Square ();

  public: 
    Square (uint32_t, uint32_t, Direction, SDL_Color);
    Square (const Square &);
    Square (Square &&);
    Square & operator= (const Square &);
    Square & operator= (Square &&);

    virtual void draw (SDL_Renderer *);
};
