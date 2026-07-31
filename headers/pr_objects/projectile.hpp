#pragma once

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"

struct Projectile {
  MemDir2 position;
  MemDir2 velocity;
  MemDir2 force;
  MemDir2 collision_normal;
  MemDir2 direction_to_new_position;

  float multiplicative_distance;
  float mass;
  float radio;

  //  0: movible -> The external forces adn velocities don't affect it.
  //  1: normal_presence -> Denote if the collision was made, so the force 
  //                          can be corrected and the friction applied
  //  2...: indefined.
  uint8_t config;

  Projectile () noexcept;
  Projectile (AngDir2 center, float radio, float mass, bool movible = true) noexcept;

  void calculate_future_movement (Global* glb, const AngDir2 & extrenal_forces);
  void make_movement ();
};
