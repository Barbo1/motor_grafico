#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include "../../../../headers/concepts/collition.hpp"

Dir2 collition_point (Physical & ob, Line& line) {
  Square * sq;
  Circle * cir;

  if ((sq = dynamic_cast<Square*>(&ob))) {
    return collition_point (*sq, line);

  } else if ((cir = dynamic_cast<Circle*>(&ob))) {
    return collition_point (*cir, line);
  }

  return Dir2 {0.f, 0.f};
}
