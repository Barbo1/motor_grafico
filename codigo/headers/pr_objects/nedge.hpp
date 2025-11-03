#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"

class NEdge: public Physical {
  private:
    std::vector<Dir2> points;
    Visualizer<D2FIG> texture;
    Global* glb;

  public: 
    NEdge ();
    NEdge (Global* glb, const std::vector<Dir2> & points, SDL_Color* color = nullptr);
    NEdge (const NEdge &);
    NEdge (NEdge &&);
    NEdge & operator= (const NEdge &);
    NEdge & operator= (NEdge &&);

    void set_texture (Visualizer<D2FIG>);
    Visualizer<D2FIG> get_texture () const;

    void draw ();

    friend Physical;
    friend bool test_collition (Physical &, Physical &);
    friend void resolve_collition (Physical &, Physical &);
    friend Dir2 collition_point (Physical &, Physical &);
};
