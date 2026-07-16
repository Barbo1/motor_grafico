#include "../../../headers/primitives/arena.hpp"
#include <cstdlib>

ArenaConstexFlag Arena::get_context() {
  return ArenaConstexFlag {
    .context_current = this->curr_ptr,
    .context_remaining = this->remaining
  };
}
