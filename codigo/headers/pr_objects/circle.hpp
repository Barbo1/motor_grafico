#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/texture.hpp"

#include <SDL2/SDL.h>

class Circle: public Texture, public Physical {
  private:
    uint32_t radio;

  public: 
    Circle (
      SDL_Renderer* render, uint32_t radio, AngularDirection center, SDL_Color color, float density = 0, 
      float elasticity = 0, float friction = 0, bool movible = true, bool colidable = true
    );
    Circle (const Circle &);
    Circle (Circle &&);
    Circle & operator= (const Circle &);
    Circle & operator= (Circle &&);
    ~Circle ();

    void set_radio (uint32_t);
    uint32_t get_radio ();

    void set_color (SDL_Color);
    SDL_Color get_color ();

    virtual void draw (SDL_Renderer *);
};
