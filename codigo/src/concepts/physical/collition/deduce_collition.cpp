#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include <cstdint>
#include <cmath>

void deduce_collition (Physical & ob1, Physical & ob2) {
  Square * sq;
  Circle * cir;

  if (dynamic_cast<Square*>(&ob1) && dynamic_cast<Square*>(&ob2)) {
    float mass_1 = ob1.get_mass(), mass_2 = ob2.get_mass();
    bool pos = std::abs(ob1.position.x - ob2.position.x) < sq->width;
    float * vel_elem1 = (pos ? &(ob1._velocity.y) : &(ob1._velocity.x));
    float * vel_elem2 = (pos ? &(ob2._velocity.y) : &(ob2._velocity.x));

    float denom = 1.f / (mass_1 + mass_2);

    float coef_3 = (mass_1 - mass_2) * denom, 
          coef_2 = 2.f * mass_2 * denom, 
          coef_1 = 2.f * mass_1 * denom;

    float new_elem = *vel_elem1 * coef_3 + *vel_elem2 * coef_2;
    *vel_elem2 = (ob2._movible ? *vel_elem1 * coef_1 - *vel_elem2 * coef_3 : 0);
    *vel_elem1 = (ob1._movible ? new_elem : 0);

  } else if (dynamic_cast<Circle*>(&ob1) && dynamic_cast<Circle*>(&ob2)) {
    AngDir2 n = (ob1.position - ob2.position).fnormalize();
    float mass_1 = ob1.get_mass(), mass_2 = ob2.get_mass();
    float p = (ob1._velocity - ob2._velocity) * n * 2.f / (mass_1 + mass_2);
    ob1._velocity -= n * (p * mass_2 * ob1._movible);
    ob2._velocity += n * (p * mass_1 * ob2._movible);

  } else if (
      ((sq = dynamic_cast<Square*>(&ob1)) && (cir = dynamic_cast<Circle*>(&ob2))) || 
      ((sq = dynamic_cast<Square*>(&ob2)) && (cir = dynamic_cast<Circle*>(&ob1)))) {
    float mass_1 = sq->get_mass(), mass_2 = cir->get_mass();
    AngDir2 diff = cir->position - sq->position;
    Dir2 diffa = diff.abs();

    bool pos = diffa.x < sq->width;
    if (pos || diffa.y < sq->height) {
      float * vel_elem1 = (pos ? &(sq->_velocity.y) : &(sq->_velocity.x));
      float * vel_elem2 = (pos ? &(cir->_velocity.y) : &(cir->_velocity.x));

      float denom = 1.f / (mass_1 + mass_2);

      float coef_3 = (mass_1 - mass_2) * denom, 
            coef_2 = 2.f * mass_2 * denom, 
            coef_1 = 2.f * mass_1 * denom;

      float new_elem = *vel_elem1 * coef_3 + *vel_elem2 * coef_2;
      *vel_elem2 = (cir->_movible ? *vel_elem1 * coef_1 - *vel_elem2 * coef_3 : 0);
      *vel_elem1 = (sq->_movible ? new_elem : 0);

    } else {
      AngDir2 n = AngDir2 {
        diff.x - static_cast<float>(sq->width * (( (*(int32_t*)&diff.x & 0x80000000) >> 30) + 1)),
        diff.y - static_cast<float>(sq->height * (( (*(int32_t*)&diff.y & 0x80000000) >> 30) + 1)),
        0
      }.fnormalize();
      float p = (sq->_velocity - cir->_velocity) * n * 2.f / (mass_1 + mass_2);
      n *= p;

      sq->_velocity -= n * (mass_2 * sq->_movible);
      cir->_velocity += n * (mass_1 * cir->_movible);
    }
  }
}


/*
    AngDir2 diff = cir->position - sq->position;

    AngDir2 col = {
      std::abs(diff.x) > sq->width ? 
        sq->position.x + (sq->width * (( (*(int32_t*)&diff.x & (int32_t)0x80000000) >> 30) + 1)) :
        cir->position.x,
      std::abs(diff.y) > sq->height ? 
        sq->position.y + (sq->height * (( (*(int32_t*)&diff.y & (int32_t)0x80000000) >> 30) + 1)) :
        cir->position.y,
      0
    };
 */
