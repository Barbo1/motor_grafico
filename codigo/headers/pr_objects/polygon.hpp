#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/texture.hpp"

class NEdge: public Texture, public Physical {
  private:
    std::vector<Direction> _points;

    NEdge ();

  public: 
    NEdge (float);
    NEdge (const NEdge &);
    NEdge (NEdge &&);
    NEdge & operator= (const NEdge &);
    NEdge & operator= (NEdge &&);

    virtual void draw (SDL_Renderer *);
};
