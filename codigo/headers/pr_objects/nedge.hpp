#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/texture.hpp"

class NEdge: public Physical, public Texture {
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
