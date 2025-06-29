#pragma once

#include "../pr_concepts/primitives.hpp"

/* ----- */
/* Hole. */
/* ----- */

class Impulse {
  private:
    Position position;
    Direction force;
    float radius;
    Function intensity_distriution;

  public:
};

class Hole: public Impulse {
  public:
};

class Fan: public Impulse {
  public:
};
