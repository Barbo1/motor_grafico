#pragma once 

#include "./physical.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/polygon.hpp"
#include "../pr_objects/square.hpp"

/* deduce in how many steps the objects will colide:
 *  - if the returned value is t < 0, the objects will not colide in the future,
 *  - if the returned value is 0, the objects have colide now,
 *  - if the returned value is t > 0, the objects will colide in time t.
 * */
float steps_to_collide (const Physical &, const Physical &);

/* take two objects and generate the collition. Return true if the collition 
 * has occurred, and false if not. 
 * */
bool calculate_collition (const Physical &, const Physical &);
    
bool collition (const Physical &, const Physical &);
