#include "../../../headers/pr_objects/projectile.hpp"

void Projectile::make_movement() {
  this->position.store(Dir2(this->direction_to_new_position).madd(
    this->multiplicative_distance, 
    Dir2(this->position)
  ));
}
