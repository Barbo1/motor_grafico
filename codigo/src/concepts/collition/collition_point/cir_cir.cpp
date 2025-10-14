#include "../../../../headers/pr_objects/circle.hpp"

Dir2 collition_point (Circle& cir1, Circle& cir2) {
  AngDir2 n = (cir1.position - cir2.position).normalize();
  return cir2.position + n * cir2.radio;
}
