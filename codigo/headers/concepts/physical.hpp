#pragma once 

#include <vector>

#include "./primitives.hpp"

#define DRAW_RATE 0.036666
#define MOVEMENT_BOUND 0.0001

class Physical {
  private:
    /* forces and velocity. */
    AngDir2 _velocity;
    AngDir2 _force;

    bool _movible; /* The external forces adn velocities don't affect it.  */
    bool _colidalble; /* The cilition with objects don't have effect. */

  protected:
    float _area; /* px^2 */
    float _density; /* kg/px^2 */
    float _f_s; /* static fritction. */
    float _f_k; /* kinetic fritction. */

    AngDir2 position;

  public:
    Physical (
      AngDir2 position, float density, float area, float f_s, 
      float f_k, bool movible, bool colidable
    );
    Physical ();

    virtual void set_position (AngDir2);
    virtual AngDir2 get_position () const;

    virtual void set_force (const AngDir2 &);
    virtual void add_force (const AngDir2 &);
    virtual AngDir2 get_force () const;

    virtual void set_velocity (const AngDir2 &);
    virtual void add_velocity (const AngDir2 &);
    virtual AngDir2 get_velocity () const;

    virtual float get_mass () const;

    virtual void calculate_movement(const std::vector<AngDir2*> & external_forces);

    friend bool test_collition (Physical &, Physical &);
    friend void resolve_collition (Physical &, Physical &);
    friend void correct_collition (Physical &, Physical &);
    friend Dir2 collition_point (Physical &, Physical &);
};
