#pragma once 

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"

#define DRAW_RATE 5.0f
#define MOVEMENT_COEFITIENT 100.f
#define MOVEMENT_BOUND 0.01

class Physical {
  protected:
    AngDir2 position;
    AngDir2 _velocity;
    AngDir2 _force;
    AngDir2 _collition_normal;

    float _area; /* px^2 */
    float _density; /* kg/px^2 */
    float _f_k; /* kinetic fritction. */
    float _acc_f_k;

    bool _movible; /* The external forces adn velocities don't affect it. */
    bool _colidalble; /* The colition with objects don't have effect. */
    bool _normal_presence; /* Denote if the collition was made, so the force 
                            * can be corrected and the friction applied */

    Global* glb;

  public:
    Physical (
      Global* glb, AngDir2 position, float density, float area, float f_k, bool movible, bool colidable
    ) noexcept;
    Physical () noexcept;
    virtual ~Physical () = default;

    virtual float get_mass () const;
    virtual void set_position (AngDir2);
    virtual AngDir2 get_position () const;

    virtual void set_force (const AngDir2 &);
    virtual void add_force (const AngDir2 &);
    virtual AngDir2 get_force () const;

    virtual void set_velocity (const AngDir2 &);
    virtual void add_velocity (const AngDir2 &);
    virtual AngDir2 get_velocity () const;

    virtual void calculate_movement (const AngDir2 & extrenal_forces);

    friend bool test_collition (Physical &, Physical &);
    friend void resolve_collition (Physical &, Physical &);
    friend void correct_collition (Physical &, Physical &);
    friend Dir2 collition_point (Physical &, Physical &);
};
