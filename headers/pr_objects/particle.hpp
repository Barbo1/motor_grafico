#pragma once

#include "../concepts/physical.hpp"
#include "../primitives/global.hpp"
#include "../primitives/vectors.hpp"

#include <cstdint>

class Circle;
class Line;
class Square;
template<std::size_t N> class NEdge;

class Particle {
  private:
    AngDir2 _position;
    AngDir2 _velocity;
    AngDir2 _force;
    float _mass;
    float _radio;
    Global* glb;

  public: 
    Particle () noexcept;
    Particle (Global* glb, uint32_t radio, const AngDir2& center, float mass) noexcept;
    Particle (const Particle &) noexcept;
    Particle (Particle &&) noexcept;
    Particle & operator= (const Particle &) noexcept;
    Particle & operator= (Particle &&) noexcept;
    
    void add_position (const AngDir2&);
    void add_velocity (const AngDir2 &);
    void add_force (const AngDir2 &);

    void set_radio (float);
    void set_position (const AngDir2&);
    void set_velocity (const AngDir2 &);
    void set_force (const AngDir2 &);

    float get_mass () const;
    float get_radio () const;
    AngDir2 get_position () const;
    AngDir2 get_velocity () const;
    AngDir2 get_force () const;

    friend bool test_collition (const Line&, const Particle&);
    friend bool test_collition (const Particle&, Physical&);
    friend bool test_collition (const Particle&, const Circle&);
    friend bool test_collition (const Particle&, const Square&);
    template<std::size_t N> friend bool test_collition (const Particle&, const NEdge<N>&);

    friend void resolve_collition (Particle&, Line&);
    friend void resolve_collition (Particle&, Physical&);
    friend void resolve_collition (Particle&, Square&);
    friend void resolve_collition (Particle&, Circle&);
    template<std::size_t N> friend void resolve_collition (Particle&, NEdge<N>&);

    friend void correct_collition (Particle&, Line&);
    friend void correct_collition (Particle&, Physical&);
    friend void correct_collition (Particle&, Square&);
    friend void correct_collition (Particle&, Circle&);
    template<std::size_t N> friend void correct_collition (Particle&, NEdge<N>&);
};
