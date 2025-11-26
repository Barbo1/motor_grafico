#include "../../../../headers/primitives/global.hpp"
#include <cstdint>

uint32_t Global::get_random () {
  this->xN = this->a * this->xN + this->c;
  return this->xN;
}
