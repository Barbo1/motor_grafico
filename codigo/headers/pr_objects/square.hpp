#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include <cstdint>

class Square: public Physical {
  private:
    int32_t height;
    int32_t width;
    Visualizer<D2FIG> texture;

    Square ();

  public: 
    Square (
      SDL_Renderer* render, uint32_t height, uint32_t width, AngDir2 center, 
      SDL_Color color, float density = 0, float f_s = 0, float f_k = 0, 
      bool movible = true, bool colidable = true
    );
    Square (uint32_t, uint32_t, Dir2, SDL_Color);
    Square (const Square &);
    Square (Square &&);
    Square & operator= (const Square &);
    Square & operator= (Square &&);

    void draw (SDL_Renderer *);

    friend Physical;
    friend bool test_collition (Physical &, Physical &);
    friend void resolve_collition (Physical &, Physical &);
    friend void correct_collition (Physical &, Physical &);
    friend Dir2 collition_point (Physical &, Physical &);
};
