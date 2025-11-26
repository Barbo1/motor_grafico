#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include "../../../../headers/concepts/collition.hpp"

Dir2 collition_point (Physical & ob1, Physical & ob2) {
  Square * sq1, * sq2;
  Circle * cir1, * cir2;

  if ((sq1 = dynamic_cast<Square*>(&ob1)) && (sq2 = dynamic_cast<Square*>(&ob2))) {
    return collition_point (*sq1, *sq2);

  } else if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (cir2 = dynamic_cast<Circle*>(&ob2))) {
    return collition_point (*cir1, *cir2);

  } else if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (sq1 = dynamic_cast<Square*>(&ob2))) {
    return collition_point (*cir1, *sq1);

  } else if ((cir1 = dynamic_cast<Circle*>(&ob2)) && (sq1 = dynamic_cast<Square*>(&ob1))) {
    return collition_point (*sq1, *cir1);

  }
  return Dir2 {0.f, 0.f};
}
