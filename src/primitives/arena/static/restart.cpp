#include "../../../../headers/primitives/arena.hpp"
#include <cstdlib>

void Arena::restart() {
  this->curr_ptr = this->init_ptr;
  this->remaining = this->size;
}
