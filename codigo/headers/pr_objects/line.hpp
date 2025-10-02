#pragma once

#include "../concepts/physical.hpp"

#include <SDL2/SDL.h>

class Line {
  private:
    float slope;
    float deviation;

  public: 
    Line ();
    Line (float slope, float deviation);
    Line (const Line &);
    Line (Line &&);
    Line & operator= (const Line &);
    Line & operator= (Line &&);

    float get_slope ();
    float get_deviation ();

    void set_slope (float);
    void set_deviation (float);

    friend bool test_collition (Physical &, Line &);
    friend void resolve_collition (Physical &, Line &);
};
