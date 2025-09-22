#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"

class NEdge: public Physical {
  private:
    Visualizer<D2FIG> texture;
    std::vector<Dir2> _points;

  public: 
    NEdge ();
    NEdge (std::vector<Dir2> & _points);
    NEdge (const NEdge &);
    NEdge (NEdge &&);
    NEdge & operator= (const NEdge &);
    NEdge & operator= (NEdge &&);

    void draw (SDL_Renderer *);

    friend Physical;
    friend bool test_collition (Physical &, Physical &);
    friend void deduce_collition (Physical &, Physical &);
    friend void correct_collition (Physical &, Physical &);
    friend Dir2 collition_point (Physical &, Physical &);
};
