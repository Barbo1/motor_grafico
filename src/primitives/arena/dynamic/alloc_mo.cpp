#include "../../../../headers/primitives/arena.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include <cstdlib>
#include <cstdint>

MaskObject* DynamicalArena::alloc_mo () {
  if (this->freed_mo != nullptr) {
    MaskObject* current = this->freed_mo;
    this->freed_mo = this->freed_mo->next;
    return current;
  } else {
    uintptr_t iptr = reinterpret_cast<uint64_t>(this->curr_ptr);
    uintptr_t align_offset = ~(iptr - 1) & (alignof(MaskObject) - 1);
    std::size_t complete_size = sizeof(MaskObject) + align_offset;
    
    if (this->remaining < complete_size) {
      this->remaining = this->size;
      this->curr_ptr = (std::byte*)malloc(this->size);
      this->ptr_blocks.push_back(this->curr_ptr);
      this->blocks++;

      iptr = reinterpret_cast<uint64_t>(this->curr_ptr);
      align_offset = ~(iptr - 1) & (alignof(MaskObject) - 1);
      complete_size = sizeof(MaskObject) + align_offset;
    }

    this->curr_ptr += complete_size;
    this->remaining -= complete_size;
    return reinterpret_cast<MaskObject*>(iptr + align_offset);
  }
}
/**
class DynamicalArena {
  private:
    std::byte* init_ptr;
    std::vector<std::byte> ptr_blocks;
    std::size_t size, remaining, blocks;

    // pointers to free memory.
    MaskObject* ptr_mo;
  public:
    DynamicalArena(std::size_t size);
    ~DynamicalArena();
  
    MaskObject* alloc_mo (std::size_t size);
    void free_mo (MaskObject*);
};
 */
