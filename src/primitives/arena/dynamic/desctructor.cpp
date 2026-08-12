#include "../../../../headers/primitives/arena.hpp"
#include <cstdlib>

DynamicalArena::~DynamicalArena() {
  for (std::size_t i = 0; i < this->blocks; i++)
    free(this->ptr_blocks[i]);
}
