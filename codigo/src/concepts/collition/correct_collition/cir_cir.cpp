#include "../../../../headers/pr_objects/circle.hpp"

void correct_collition (Circle& cir1, Circle& cir2) {
  cir1.position = cir2.position;
  cir1.position += (cir1.position - cir2.position).normalize() * (cir1.radio + cir2.radio);
}
