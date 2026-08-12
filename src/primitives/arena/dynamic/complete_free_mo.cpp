#include "../../../../headers/primitives/arena.hpp"
#include "../../../../headers/primitives/types_definition.hpp"
#include <cstdlib>

void DynamicalArena::complete_free_mo (MaskObject* obj) {
  MaskObject* iter = obj;
  while (iter->next != nullptr)
    iter = iter->next;
  iter->next = this->freed_mo;
  this->freed_mo = obj;
}
