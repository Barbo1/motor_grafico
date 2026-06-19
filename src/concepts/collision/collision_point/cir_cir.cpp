#include "../../../../headers/pr_objects/circle.hpp"

Dir2 collision_point (const Circle& cir1, const Circle& cir2) {
  AngDir2 n = (cir1.position - cir2.position).normalize();
  return n.madd(cir2.radio, cir2.position);
}
