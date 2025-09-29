#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include <cstdint>
#include <cmath>

void correct_collition (Physical & ob1, Physical & ob2) {
  Square * sq1, * sq2;
  Circle * cir1, * cir2;
  int a;

  if ((sq1 = dynamic_cast<Square*>(&ob1)) && (sq2 = dynamic_cast<Square*>(&ob2))) {
    Dir2 diff = (ob1.position - ob2.position).abs();
    diff.x -= sq2->width + sq1->width;
    diff.y -= sq2->height + sq1->height;
    int pos = diff.x < diff.y;
    *(&sq1->position.x + pos) += *(&diff.x + pos);
    return;

  } else if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (cir2 = dynamic_cast<Circle*>(&ob2))) {
    float mass_1 = cir1->get_mass(), mass_2 = cir2->get_mass();
    AngDir2 n = (cir1->position - cir2->position).fnormalize();
    cir1->position = cir2->position;
    cir1->position += n * (cir1->radio + cir2->radio);
    return;

  } else if (
      (a = (cir1 = dynamic_cast<Circle*>(&ob1)) && (sq1 = dynamic_cast<Square*>(&ob2))) || 
      (sq1 = dynamic_cast<Square*>(&ob1)) && (cir1 = dynamic_cast<Circle*>(&ob2))
    ) {

    float mass_1 = sq1->get_mass(), mass_2 = cir1->get_mass();
    AngDir2 diff = sq1->position - cir1->position;
    Dir2 diffa = diff.abs();

    bool pos = diffa.x < sq1->width;
    if (pos || diffa.y < sq1->height) {
      *(&ob1.position.x + pos) += *(&diffa.x + pos) - (cir1->radio + (pos ? sq1->height : sq1->width));
      return;
    } else {
      AngDir2 b = AngDir2 {
        diff.x - static_cast<float>(sq1->width * (((int32_t) (*(uint32_t*)&diff.x & 0x80000000) >> 30) + 1)),
        diff.y - static_cast<float>(sq1->height * (((int32_t) (*(uint32_t*)&diff.y & 0x80000000) >> 30) + 1)),
        0
      };
      b -= b.fnormalize() * cir1->radio;
      cir1->position += b * a;
      sq1->position -= b * (1-a);
      return;
    }
  }
}
