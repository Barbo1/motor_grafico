#include "../../../headers/concepts/physical.hpp"
#include "../../../headers/pr_objects/circle.hpp"
#include "../../../headers/pr_objects/square.hpp"
#include "../../../headers/pr_objects/nedge.hpp"
#include <cstdint>
#include <cmath>

Dir2 collition_point (Physical & ob1, Physical & ob2) {
  Circle * cir1, * cir2;
  Square * sq1, * sq2;
  bool b;

  if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (cir2 = dynamic_cast<Circle*>(&ob2))) {
    Dir2 ret = (cir1->position - cir2->position).normalize();
    ret *= cir2->radio;
    return ret;
  } else if ((cir1 = dynamic_cast<Circle*>(&ob1)) && (sq1 = dynamic_cast<Square*>(&ob2))) {
    Dir2 diff = cir1->position - sq1->position;

    return {
      std::abs(diff.x) > sq1->width ? 
        sq1->position.x + (sq1->width * (( (*(int32_t*)&diff.x & (int32_t)0x80000000) >> 30) + 1)) :
        cir1->position.x,
      std::abs(diff.y) > sq1->height ? 
        sq1->position.y + (sq1->height * (( (*(int32_t*)&diff.y & (int32_t)0x80000000) >> 30) + 1)) :
        cir1->position.y
    };
  } else if ((sq1 = dynamic_cast<Square*>(&ob1)) && (cir1 = dynamic_cast<Circle*>(&ob2))) {
    Dir2 diff = cir1->position - sq1->position;

    Dir2 ret = {
      std::abs(diff.x) > sq1->width ?
        sq1->position.x + (sq1->width * (( (*(int32_t*)&diff.x & (int32_t)0x80000000) >> 30) + 1)) :
        cir1->position.x,
      std::abs(diff.y) > sq1->height ?
        sq1->position.y + (sq1->height * (( (*(int32_t*)&diff.y & (int32_t)0x80000000) >> 30) + 1)) :
        cir1->position.y
    };
    ret -= cir1->position;
    ret = ret.normalize();
    ret *= cir1->radio;
    ret += cir1->position;
    return ret;
  } else if ((sq1 = dynamic_cast<Square*>(&ob1)) && (sq2 = dynamic_cast<Square*>(&ob2))) {
    
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
/*
float * vel_elem1 = (pos ? &(ob1._velocity.y) : &(ob1._velocity.x));
float * vel_elem2 = (pos ? &(ob2._velocity.y) : &(ob2._velocity.x));
* */
