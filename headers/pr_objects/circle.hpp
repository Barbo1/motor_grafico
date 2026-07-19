#pragma once

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"

struct Circle {
  MemDir2 position;
  MemDir2 velocity;
  MemDir2 force;
  MemDir2 collision_normal;

  float radio;
  float area; /* px^2 */
  float density; /* kg/px^2 */
  float f_k; /* kinetic fritction. */
  float acc_f_k;

  //  0: movible -> The external forces adn velocities don't affect it.
  //  1: normal_presence -> Denote if the collision was made, so the force 
  //                          can be corrected and the friction applied
  //  2...: indefined.
  uint8_t config;

  Circle () noexcept;
  Circle (Dir2 center, float radio, float density, float f_k = 0, bool movible = true) noexcept;

  float get_mass () const;

  void calculate_movement(Global* glb, const AngDir2 & extrenal_forces);
};
