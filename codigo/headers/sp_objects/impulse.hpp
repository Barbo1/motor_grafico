#pragma once

#include "../pr_concepts/primitives.hpp"

/* ----- */
/* Hole. */
/* ----- */

class Impulse {
  private:
    Position position;
    Dir2 force;
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
