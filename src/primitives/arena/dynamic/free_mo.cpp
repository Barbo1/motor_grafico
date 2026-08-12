#include "../../../../headers/primitives/arena.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include <cstdlib>

void DynamicalArena::free_mo (MaskObject* obj) {
  obj->next = this->freed_mo;
  this->freed_mo = obj;
}
