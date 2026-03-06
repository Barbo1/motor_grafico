#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

uint32_t ImageModifier::height () {
  if (this->texture == nullptr)
    return 0;
  else
    return this->texture->h;
}
