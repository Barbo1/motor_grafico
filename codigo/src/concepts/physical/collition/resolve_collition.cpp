#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include <cstdint>
#include <cmath>

void resolve_collition (Physical & ob1, Physical & ob2) {
  Square * sq1, * sq2;
  Circle * cir1, * cir2;
  int a;

  if ((sq1 = dynamic_cast<Square*>(&ob1)) && (sq2 = dynamic_cast<Square*>(&ob2))) {
    Dir2 diff = (ob1.position - ob2.position).abs();
    diff.x -= sq2->width + sq1->width;
    diff.y -= sq2->height + sq1->height;

    int pos = diff.x < diff.y;
    float * vel_elem1 = &sq1->_velocity.x + pos;
    float * vel_elem2 = &sq2->_velocity.x + pos;

    float mass_1 = ob1.get_mass(), mass_2 = ob2.get_mass();
    float denom = 1.f / (mass_1 + mass_2);
    float coef_3 = (mass_1 - mass_2);

    float new_elem = (*vel_elem1 * coef_3 + 2.f * *vel_elem2 * mass_2) * denom;
    *vel_elem2 = (2.f * *vel_elem1 * mass_1 - *vel_elem2 * coef_3) * denom * ob2._movible;
    *vel_elem1 = new_elem * ob1._movible;

    *(&sq1->position.x + pos) += *(&diff.x + pos);

    return;

  } else if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (cir2 = dynamic_cast<Circle*>(&ob2))) {
    float mass_1 = cir1->get_mass(), mass_2 = cir2->get_mass();
    AngDir2 n = (cir1->position - cir2->position).fnormalize();
    float p = (cir1->_velocity - cir2->_velocity) * n * 2.f / (mass_1 + mass_2);
    cir1->_velocity -= n * (p * mass_2 * cir1->_movible);
    cir2->_velocity += n * (p * mass_1 * cir2->_movible);

    cir1->position = cir2->position;
    cir1->position += n * (cir1->radio + cir2->radio);
    return;

  } else if (
      (a = (cir1 = dynamic_cast<Circle*>(&ob1)) && (sq1 = dynamic_cast<Square*>(&ob2))) || 
      (sq1 = dynamic_cast<Square*>(&ob1)) && (cir1 = dynamic_cast<Circle*>(&ob2))
    ) {

    float mass_1 = sq1->get_mass(), mass_2 = cir1->get_mass();
    AngDir2 diff = cir1->position - sq1->position;
    Dir2 diffa = diff.abs();

    bool pos = diffa.x < sq1->width;
    if (pos || diffa.y < sq1->height) {
      float * vel_elem1 = &sq1->_velocity.x + pos;
      float * vel_elem2 = &cir1->_velocity.x + pos;

      float denom = 1.f / (mass_1 + mass_2);

      float coef_3 = (mass_1 - mass_2) * denom, 
            coef_2 = 2.f * mass_2 * denom, 
            coef_1 = 2.f * mass_1 * denom;

      float new_elem = *vel_elem1 * coef_3 + *vel_elem2 * coef_2;
      *vel_elem2 = (*vel_elem1 * coef_1 - *vel_elem2 * coef_3) * cir1->_movible;
      *vel_elem1 = new_elem * sq1->_movible;

      *(&ob1.position.x + pos) += *(&diffa.x + pos) - (cir1->radio + (pos ? sq1->height : sq1->width));

      return;

    } else {
      AngDir2 col_point = AngDir2 {
        sq1->position.x + static_cast<float>(sq1->width * (((int32_t) (*(uint32_t*)&diff.x & 0x80000000) >> 30) + 1)),
        sq1->position.y + static_cast<float>(sq1->height * (((int32_t) (*(uint32_t*)&diff.y & 0x80000000) >> 30) + 1)),
        0
      };
      AngDir2 b = col_point - cir1->position;
      AngDir2 n = b.fnormalize();
      float p = (cir1->_velocity - sq1->_velocity) * n * 2.f / (mass_1 + mass_2);

      sq1->_velocity += n * (p * mass_2 * sq1->_movible);
      cir1->_velocity -= n * (p * mass_1 * cir1->_movible);

      b -= n * cir1->radio;
      cir1->position += b * a;
      sq1->position -= b * (1-a);

      return;
    }
  }
}
