#include "../../../../headers/primitives/global.hpp"

void Global::set_ambient_light (float per) {
  this->darkness_color.a = static_cast<Uint8>(per * 255.f);
}
