#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"

class NEdge: public Physical {
  private:
    std::vector<Dir2> points;
    Visualizer<D2FIG> texture;

  public: 
    NEdge ();
    NEdge (SDL_Renderer* render, const std::vector<Dir2> & points, SDL_Color* color = nullptr);
    NEdge (const NEdge &);
    NEdge (NEdge &&);
    NEdge & operator= (const NEdge &);
    NEdge & operator= (NEdge &&);

    void set_texture (Visualizer<D2FIG>);
    Visualizer<D2FIG> get_texture ();

    void draw (SDL_Renderer *);

    friend Physical;
    friend bool test_collition (Physical &, Physical &);
    friend void resolve_collition (Physical &, Physical &);
    friend void correct_collition (Physical &, Physical &);
    friend Dir2 collition_point (Physical &, Physical &);
};
