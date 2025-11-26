#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/collition.hpp"

void correct_collition (Physical & ob, Line& line) {
  Square * sq;
  Circle * cir;

  if ((sq = dynamic_cast<Square*>(&ob))) {
    correct_collition (*sq, line);
    return;

  } else if ((cir = dynamic_cast<Circle*>(&ob))) {
    correct_collition (*cir, line);
    return;
  }
}
