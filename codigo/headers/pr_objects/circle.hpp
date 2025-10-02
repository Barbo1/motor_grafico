#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"

#include <SDL2/SDL.h>
#include <cstdint>

class Circle: public Physical {
  private:
    Visualizer<D2FIG> texture;
    uint32_t radio;

  public: 
    Circle ();
    Circle (
      SDL_Renderer* render, uint32_t radio, AngDir2 center, float density = 0, 
      float f_s = 0, float f_k = 0, bool movible = true, bool colidable = true, 
      SDL_Color* color = nullptr
    );
    Circle (const Circle &);
    Circle (Circle &&);
    Circle & operator= (const Circle &);
    Circle & operator= (Circle &&);

    void set_radio (uint32_t);
    uint32_t get_radio ();

    void draw (SDL_Renderer *);

    friend Physical;
    friend bool test_collition (Physical &, Physical &);
    friend void resolve_collition (Physical &, Physical &);
    friend void correct_collition (Physical &, Physical &);
    friend Dir2 collition_point (Physical &, Physical &);
};
