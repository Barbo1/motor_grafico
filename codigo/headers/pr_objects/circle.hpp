#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/texture.hpp"

#include <SDL2/SDL.h>

class Circle: public Physical, public Texture {
  private:
    uint32_t radio;

  public: 
    Circle ();
    Circle (
      SDL_Renderer* render, uint32_t radio, AngDir2 center, SDL_Color color, float density = 0, 
      float elasticity = 0, float f_s = 0, float f_k = 0, bool movible = true, bool colidable = true
    );
    Circle (const Circle &);
    Circle (Circle &&);
    Circle & operator= (const Circle &);
    Circle & operator= (Circle &&);
    ~Circle ();

    void set_radio (uint32_t);
    uint32_t get_radio ();

    void draw (SDL_Renderer *);
};
