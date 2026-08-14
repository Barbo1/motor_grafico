#include "../../../../headers/primitives/arena.hpp"
#include <cstdlib>

DynamicalArena::~DynamicalArena() {
  for (auto ptr: this->ptr_blocks) 
    free(ptr);
}
