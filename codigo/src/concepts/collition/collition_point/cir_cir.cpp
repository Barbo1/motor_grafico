#include "../../../../headers/pr_objects/circle.hpp"

Dir2 collition_point (Circle& cir1, Circle& cir2) {
  AngDir2 n = (cir1.get_position() - cir2.get_position()).normalize();
  return n.madd(cir2.get_radio(), cir2.get_position());
}
