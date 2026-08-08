#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_direction (Circle& cir1, Circle& cir2) {
  return (Dir2(cir2.position) - Dir2(cir1.position)).normalize();
}
