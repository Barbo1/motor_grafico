#pragma once 

#include <vector>

#include "./primitives.hpp"

#define DRAW_RATE 0.036666
#define MOVEMENT_BOUND 0.0001

enum ObjectType {
  CIRCLE,
  SQUARE,
  POLYGON
};

/* some clarifications:
 * - density * area == weight 
 * - all objects are collidable
 * - for consistency, the position is kept out.
 * */

class Physical {
  private:
    /* forces and velocity. */
    AngDir2 _velocity;
    AngDir2 _force;

    bool _movible; /* The external forces adn velocities don't affect it.  */
    bool _colidalble; /* The cilition with objects don't have effect. */
    int _index; /* For printing proposes. Greater means first showed. */

    Dir2 _gcenter;

  protected:
    float _area; /* px^2 */
    float _density; /* kg/px^2 */
    float _elasticity; /* Near 0 means rigid, near 0 marks complete elastic. */
    float _f_s; /* static fritction. */
    float _f_k; /* kinetic fritction. */

    AngDir2 position;

  public:
    Physical (
        AngDir2 position, float density, float area, 
        float elasticity, float f_s, float f_k, bool movible, 
        bool colidable
    );
    Physical ();

    void set_position (AngDir2);
    AngDir2 get_position () const;

    void set_force (const AngDir2 &);
    void add_force (const AngDir2 &);
    AngDir2 get_force () const;

    void set_velocity (const AngDir2 &);
    void add_velocity (const AngDir2 &);
    AngDir2 get_velocity () const;

    /* calcule the movement of the object and move it. */
    void calculate_movement(std::vector<AngDir2*> & external_forces);
};
