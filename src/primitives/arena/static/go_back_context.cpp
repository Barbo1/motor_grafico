#include "../../../../headers/primitives/arena.hpp"
#include <cstdlib>

void Arena::go_back_context(ArenaConstexFlag context) {
  this->curr_ptr = context.context_current;
  this->remaining = context.context_remaining;
}
