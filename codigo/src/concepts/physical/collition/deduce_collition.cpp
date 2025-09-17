#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include <cstdint>
#include <cmath>
#include <bit>
#include <iostream>

void deduce_collition (Physical & ob1, Physical & ob2) {
  float mass_1 = const_cast<const Physical*>(&ob1)->get_mass(), mass_2 = const_cast<const Physical*>(&ob2)->get_mass();
  Square * sq;
  Circle * cir;

  if (dynamic_cast<Square*>(&ob1) && dynamic_cast<Square*>(&ob2)) {
    float denom = 1.f / (mass_1 + mass_2);
    float coef_3 = (mass_1 - mass_2) * denom, 
          coef_2 = 2.f * mass_2 * denom, 
          coef_1 = 2.f * mass_1 * denom;
    AngDir2 vel1 = ob1._velocity * coef_3 + ob2._velocity * coef_2;
    if (ob2._movible)
      ob2._velocity = ob1._velocity * coef_1 - ob2._velocity * coef_3;
    if (ob1._movible)
      ob1._velocity = vel1;

  } else if (dynamic_cast<Circle*>(&ob1) && dynamic_cast<Circle*>(&ob2)) {
    AngDir2 n = (ob1.position - ob2.position).fnormalize();
    float p = (ob1._velocity - ob2._velocity) * n * 2.f / (mass_1 + mass_2);
    ob1._velocity -= n * (p * mass_2 * ob1._movible);
    ob2._velocity += n * (p * mass_1 * ob2._movible);

  } else if (
      ((sq = dynamic_cast<Square*>(&ob1)) && (cir = dynamic_cast<Circle*>(&ob2))) || 
      ((sq = dynamic_cast<Square*>(&ob2)) && (cir = dynamic_cast<Circle*>(&ob1)))) {
    AngDir2 diff = cir->position - sq->position;

    if (std::abs(diff.x) < sq->width || std::abs(diff.y) < sq->height) {
      float denom = 1.f / (mass_1 + mass_2);
      float coef_3 = (mass_1 - mass_2) * denom, 
            coef_2 = 2.f * mass_2 * denom, 
            coef_1 = 2.f * mass_1 * denom;
      AngDir2 vel1 = ob1._velocity * coef_3 + ob2._velocity * coef_2;
      if (ob2._movible)
        ob2._velocity = ob1._velocity * coef_1 - ob2._velocity * coef_3;
      if (ob1._movible)
        ob1._velocity = vel1;
    } else {
      AngDir2 diff = cir->position - sq->position;

      int32_t * diff_sign = (int32_t*)&diff.x;
      AngDir2 col = {
        sq->position.x + (sq->width * (( (*diff_sign & (int32_t)0x80000000) >> 30) + 1)),
        sq->position.y + (sq->height * (( (*(diff_sign + 1) & (int32_t)0x80000000) >> 30) + 1)),
        0
      };

      float mass_1 = sq->get_mass(), mass_2 = cir->get_mass();
      AngDir2 n = (col - cir->position).fnormalize();
      float p = (sq->_velocity - cir->_velocity) * n * 2.f / (mass_1 + mass_2);

      sq->_velocity -= n * (p * mass_2 * sq->_movible);
      cir->_velocity += n * (p * mass_1 * cir->_movible);
    }
  }
}


/*
    AngDir2 diff = cir->position - sq->position;

    int32_t * diff_sign = (int32_t*)&diff.x;
    AngDir2 col = {
      std::abs(diff.x) > sq->width ? 
        sq->position.x + (sq->width * (( (*diff_sign & (int32_t)0x80000000) >> 30) + 1)) :
        cir->position.x,
      std::abs(diff.y) > sq->height ? 
        sq->position.y + (sq->height * (( (*(diff_sign + 1) & (int32_t)0x80000000) >> 30) + 1)) :
        cir->position.y,
      0
    };
 */
