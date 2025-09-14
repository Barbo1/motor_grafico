#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"

bool test_collition (Physical & obj1, Physical & obj2) {
  Circle * cir1, * cir2;
  Square * sq1, * sq2;
  if ((cir1 = dynamic_cast<Circle *>(&obj1))) {
    if ((cir2 = dynamic_cast<Circle *>(&obj2))) {
      Dir2&& diff = cir1->position - cir2->position;
      uint32_t&& radio = cir1->get_radio() + cir2->get_radio();
      return (radio * radio) > diff.modulo2();
    }
  } else if ((sq1 = dynamic_cast<Square *>(&obj1))) {
    if ((sq2 = dynamic_cast<Square *>(&obj2))) {
      
    }
  }
  return false;
}

/* take two objects and generate the collition. Return true if the collition 
 * has occurred, and false if not. 
 * */
bool deduce_collition (const Physical &, const Physical &);
