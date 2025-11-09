#include "../../../../headers/primitives/global.hpp"

float Global::get_random_f01 () {
  return (this->get_random() & 1023) / 1024.f;
}
