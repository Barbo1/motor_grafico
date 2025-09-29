#pragma once

#include "../concepts/physical.hpp"

#include <SDL2/SDL.h>

class Line {
  private:
    float slope;
    float intersection;

  public: 
    Line ();
    Line (float slope, float intersection);
    Line (const Line &);
    Line (Line &&);
    Line & operator= (const Line &);
    Line & operator= (Line &&);

    float get_slope ();
    float get_intersetion ();

    void set_slope (float);
    void set_intersection (float);

    friend bool test_collition (Physical &, Line &);
    friend void resolve_collition (Physical &, Line &);
};
