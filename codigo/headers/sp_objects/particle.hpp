#pragma once

#include<vector>
#include<SDL2/SDL_pixels.h>

#include "../concepts/primitives.hpp"
#include "../concepts/physical.hpp"

/* ---------------- */
/* Particle Source. */
/* ---------------- */

const float BLUR_LIMIT = 0.001;

class Particle: public Physical {
  Position position;
  float blur; /* if blur less than BLUR_LIMIT, the particle is destroyed. */
};

class ParticleSource : private ZStackable {
  private:
    Position position;
    std::vector<SDL_Color> colors;
    std::pair<Dir2, Dir2> anglevectors;
    AngDir2 force; 
    AngDir2 velocity;
    float spawn_ratio;  /* Particles per seconds. */
    std::vector<Particle> _particles;

  public:
    void draw (SDL_Renderer *);
};

