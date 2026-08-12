#include "../../../../headers/primitives/arena.hpp"
#include <cstdint>
#include <cstdlib>

std::byte* Arena::allocd(std::size_t size, std::size_t alignment) {
  uintptr_t iptr = reinterpret_cast<uint64_t>(this->curr_ptr);
  uintptr_t align_offset = ~(iptr - 1) & (alignment - 1);
  std::size_t complete_size = size + align_offset;
  
  if (this->remaining < complete_size)
    return nullptr;
  this->curr_ptr += complete_size;
  this->remaining -= complete_size;

  return reinterpret_cast<std::byte*>(iptr + align_offset);
}
