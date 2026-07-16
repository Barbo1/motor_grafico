#include "../../../headers/primitives/arena.hpp"
#include <cstdlib>

Arena::Arena(std::size_t size) {
  this->size = size;
  this->remaining = size;
  this->init_ptr = (std::byte*)malloc(size);
  this->curr_ptr = this->init_ptr;
}
