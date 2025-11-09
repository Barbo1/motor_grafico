#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/collition.hpp"

void correct_collition (Physical & ob1, Physical & ob2) {
  Square * sq1, * sq2;
  Circle * cir1, * cir2;

  if ((sq1 = dynamic_cast<Square*>(&ob1)) && (sq2 = dynamic_cast<Square*>(&ob2))) {
    correct_collition (*sq1, *sq2);
    return;

  } else if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (cir2 = dynamic_cast<Circle*>(&ob2))) {
    correct_collition (*cir1, *cir2);
    return;

  } else if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (sq1 = dynamic_cast<Square*>(&ob2))) {
    correct_collition (*cir1, *sq1);
    return;

  } else if ((cir1 = dynamic_cast<Circle*>(&ob2)) && (sq1 = dynamic_cast<Square*>(&ob1))) {
    correct_collition (*sq1, *cir1);
    return;

  }
}
