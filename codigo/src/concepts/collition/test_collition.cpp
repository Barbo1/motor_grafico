#include "../../../headers/concepts/physical.hpp"
#include "../../../headers/pr_objects/circle.hpp"
#include "../../../headers/pr_objects/square.hpp"
#include "../../../headers/pr_objects/nedge.hpp"

bool test_collition (Physical & obj1, Physical & obj2) {
  if (!obj1._colidalble || !obj2._colidalble) 
    return false;

  Circle * cir1, * cir2;
  Square * sq1, * sq2;
  Dir2 diff;

  if ((cir1 = dynamic_cast<Circle *>(&obj1)) && (cir2 = dynamic_cast<Circle *>(&obj2))) {
    diff = cir1->position - cir2->position;
    uint32_t&& radio = cir1->radio + cir2->radio;
    return diff.modulo2() < (radio * radio);

  } else if ((sq1 = dynamic_cast<Square *>(&obj1)) && (sq2 = dynamic_cast<Square *>(&obj2))) {
    diff = (sq1->position - sq2->position).abs();
    return diff.y < sq1->height + sq2->height && diff.x < sq1->width + sq2->width;

  } else if (((cir1 = dynamic_cast<Circle *>(&obj1)) && (sq1 = dynamic_cast<Square *>(&obj2))) ||
      ((cir1 = dynamic_cast<Circle *>(&obj2)) && (sq1 = dynamic_cast<Square *>(&obj1)))) {
  
    auto max0 = [] (float a) {
      uint32_t c = std::bit_cast<uint32_t>(a);
      c &= ~((int32_t)(c & 0x80000000) >> 31);
      return std::bit_cast<float>(c);
    };

    diff = (cir1->position - sq1->position).abs();
    diff.x = max0 (diff.x - sq1->width);
    diff.y = max0 (diff.y - sq1->height);
    return diff.modulo2() < cir1->radio * cir1->radio;
  }

  return false;
}
