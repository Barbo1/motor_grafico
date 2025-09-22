#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include <algorithm>

bool test_collition (Physical & obj1, Physical & obj2) {
  if (!obj1._colidalble || !obj2._colidalble) 
    return false;

  Circle * cir1, * cir2;
  Square * sq1, * sq2;

  if ((cir1 = dynamic_cast<Circle *>(&obj1)) && (cir2 = dynamic_cast<Circle *>(&obj2))) {
    Dir2 diff = cir1->position - cir2->position;
    uint32_t&& radio = cir1->radio + cir2->radio;
    return diff.modulo2() <= (radio * radio);

  } else if ((sq1 = dynamic_cast<Square *>(&obj1)) && (sq2 = dynamic_cast<Square *>(&obj2))) {
    Dir2 diff = (sq1->position - sq2->position).abs();
    float height = sq1->height + sq2->height;
    float width = sq1->width + sq2->width;
    return diff.y < height && diff.x < width;

  } else if (((cir1 = dynamic_cast<Circle *>(&obj1)) && (sq2 = dynamic_cast<Square *>(&obj2))) ||
      ((cir1 = dynamic_cast<Circle *>(&obj2)) && (sq2 = dynamic_cast<Square *>(&obj1)))) {
    Dir2 diff = (cir1->position - sq2->position).abs();
    diff.x = std::max (diff.x - sq2->width, 0.0f);
    diff.y = std::max (diff.y - sq2->height, 0.0f);
    return diff.modulo2() < cir1->radio * cir1->radio;
  }

  return false;
}
