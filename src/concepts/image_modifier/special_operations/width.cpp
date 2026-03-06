#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

uint32_t ImageModifier::width() {
  if (this->texture == nullptr)
    return 0;
  else
    return this->texture->w;
}
