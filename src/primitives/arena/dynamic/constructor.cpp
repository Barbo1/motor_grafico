#include "../../../../headers/primitives/arena.hpp"
#include <cstdlib>

DynamicalArena::DynamicalArena(std::size_t size) {
  this->size = size;
  this->remaining = size;
  this->curr_ptr = (std::byte*)malloc(size);

  this->ptr_blocks.reserve(30);
  this->ptr_blocks.push_back(this->curr_ptr);

  this->freed_mo = nullptr;
}
