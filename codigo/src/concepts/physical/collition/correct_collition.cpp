#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/pr_objects/nedge.hpp"
#include <cstdint>
#include <cmath>

void correct_collition (Physical &, Physical &) {

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
