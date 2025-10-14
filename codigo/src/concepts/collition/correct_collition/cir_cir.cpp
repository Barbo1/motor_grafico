#include "../../../../headers/pr_objects/circle.hpp"

void correct_collition (Circle& cir1, Circle& cir2) {
  AngDir2 n = (cir1.position - cir2.position).normalize();

  cir1.position = cir2.position;
  cir1.position += n * (cir1.radio + cir2.radio);

  cir1._collition_normal = n;
  cir2._collition_normal = -n;
  
  cir1._acc_f_k = cir1._f_k * cir2._f_k;
  cir2._acc_f_k = cir1._acc_f_k;
  
  cir1._normal_presence = true;
  cir2._normal_presence = true;
}
