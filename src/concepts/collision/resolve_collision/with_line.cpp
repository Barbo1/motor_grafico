#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/collision.hpp"

void resolve_collision (Physical & ob, Line& line) {
  Square * sq;
  Circle * cir;

  if ((sq = dynamic_cast<Square*>(&ob))) {
    resolve_collision (*sq, line);
    return;

  } else if ((cir = dynamic_cast<Circle*>(&ob))) {
    resolve_collision (*cir, line);
    return;
  }
}
