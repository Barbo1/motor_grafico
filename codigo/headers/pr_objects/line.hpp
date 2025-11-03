#pragma once

#include "../concepts/physical.hpp"

#include <SDL2/SDL.h>

class Circle;
class Square;

class Line {
  private:
    float slope;
    float deviation;

  public: 
    Line ();
    Line (float slope, float deviation);
    Line (Dir2 p1, Dir2 p2);
    Line (const Line &);
    Line (Line &&);
    Line & operator= (const Line &);
    Line & operator= (Line &&);

    float get_slope () const;
    float get_deviation () const;

    void set_slope (float);
    void set_deviation (float);

    friend bool test_collition (Physical &, Line &);
    friend bool test_collition (Circle&, Line&);
    friend bool test_collition (Square&, Line&);

    friend void resolve_collition (Physical &, Line &);
    friend void resolve_collition (Circle&, Line&);
    friend void resolve_collition (Square&, Line&);

    friend void correct_collition (Physical&, Line&);
    friend void correct_collition (Circle&, Line&);
    friend void correct_collition (Square&, Line&);

    friend Dir2 collition_point (Physical&, Line&);
    friend Dir2 collition_point (Circle&, Line&);
    friend Dir2 collition_point (Square&, Line&);
};
