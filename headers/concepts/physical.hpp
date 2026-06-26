#pragma once 

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"
#include <cstdint>

#define DRAW_RATE 5.0f
#define MOVEMENT_COEFITIENT 100.f
#define ENERGY_DISIPATION 1.0f

class alignas(16) Physical {
  protected:
    Global* glb;

    MemDir2 position;
    MemDir2 velocity;
    MemDir2 force;
    MemDir2 collision_normal;

    float area; /* px^2 */
    float density; /* kg/px^2 */
    float f_k; /* kinetic fritction. */
    float acc_f_k;
    float inertia;

    uint32_t config;
    //  0: movible -> The external forces adn velocities don't affect it.
    //  1: normal_presence -> Denote if the collision was made, so the force 
    //                          can be corrected and the friction applied
    //  2...: indefined.

  public:
    Physical (
      Global* glb, AngDir2 position, float density, float area, float inertia, float f_k, 
      bool movible
    ) noexcept;
    Physical () noexcept;
    virtual ~Physical () = default;

    virtual float get_mass () const;
    virtual void set_position (const AngDir2&);
    virtual Dir2 get_position () const;

    virtual void set_velocity (const AngDir2 &);
    virtual void add_velocity (const AngDir2 &);
    virtual Dir2 get_velocity () const;

    virtual void set_force (const AngDir2 &);
    virtual void add_force (const AngDir2 &);
    virtual Dir2 get_force () const;

    virtual void calculate_movement (const AngDir2 & extrenal_forces);

    friend bool test_collision (Physical &, Physical &);
    friend void resolve_collision (Physical &, Physical &);
    friend void correct_collision (Physical &, Physical &);
    friend Dir2 collision_point (Physical &, Physical &);
};
