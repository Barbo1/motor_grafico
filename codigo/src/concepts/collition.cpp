#include "../../headers/concepts/collition.hpp"

bool collition (Physical & obj1, Physical & obj2) {
  Circle * cir1, * cir2;
  if ((cir1 = dynamic_cast<Circle *>(&obj1))) {
    if ((cir2 = dynamic_cast<Circle *>(&obj2))) {
      Direction c1 = cir1->position;
      Direction c2 = cir2->position;

      uint32_t cat1 = c1.x - c2.x;
      uint32_t cat2 = c1.y - c2.y;
      uint32_t radio_2 = cir1->get_radio() + cir2->get_radio();

      if ((radio_2 * radio_2) < (cat2 * cat2) + (cat1 * cat1)) {
        

        return true;
      }
    }
  }
  return false;
}
