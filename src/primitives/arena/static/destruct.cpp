#include "../../../../headers/primitives/arena.hpp"
#include <cstdlib>

Arena::~Arena() {
  std::free(this->init_ptr);
}
