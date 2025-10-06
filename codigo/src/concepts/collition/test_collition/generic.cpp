#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"

bool test_collition (Physical & obj1, Physical & obj2) {
  Circle * cir1, * cir2;
  Square * sq1, * sq2;

  if ((cir1 = dynamic_cast<Circle *>(&obj1)) && (cir2 = dynamic_cast<Circle *>(&obj2))) {
    return test_collition (*cir1, *cir2);

  } else if ((sq1 = dynamic_cast<Square *>(&obj1)) && (sq2 = dynamic_cast<Square *>(&obj2))) {
    return test_collition (*sq1, *sq2);
  
  } else if ((cir1 = dynamic_cast<Circle *>(&obj1)) && (sq1 = dynamic_cast<Square *>(&obj2))) {
    return test_collition (*cir1, *sq1);
  
  } else if ((cir1 = dynamic_cast<Circle *>(&obj2)) && (sq1 = dynamic_cast<Square *>(&obj1))) {
    return test_collition (*sq1, *cir1);

  }

  return false;
}
