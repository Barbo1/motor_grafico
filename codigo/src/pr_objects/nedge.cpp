#pragma once

#include "../../headers/pr_objects/nedge.hpp"

class NEdge: public Texture, public Physical {
  private:
    std::vector<Direction> _points;

  public: 
    NEdge ();
    NEdge (std::vector<Direction> & _points);
    NEdge (const NEdge &);
    NEdge (NEdge &&);
    NEdge & operator= (const NEdge &);
    NEdge & operator= (NEdge &&);

    void draw (SDL_Renderer *);
};
