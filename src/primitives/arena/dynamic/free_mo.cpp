#include "../../../../headers/primitives/arena.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include <cstdlib>

void DynamicalArena::free_mo (MaskObject* obj) {
  if (obj != nullptr) {
    obj->next = this->freed_mo;
    this->freed_mo = obj;
  }
}
