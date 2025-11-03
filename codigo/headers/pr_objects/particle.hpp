#pragma once

#include "../primitives/global.hpp"
#include "../primitives/vectors.hpp"

#include <cstdint>

template <std::size_t N>
class ParticleSource;

class Particle {
  private:
    AngDir2 _position;
    AngDir2 _velocity;
    AngDir2 _force;
    float _mass;
    float _radio;
    Global* glb;

  public: 
    Particle (Global* glb, uint32_t radio, AngDir2 center, float mass);
    Particle (const Particle &);
    Particle (Particle &&);
    Particle & operator= (const Particle &);
    Particle & operator= (Particle &&);

    void set_radio (float);
    void set_position (const AngDir2&);
    void set_velocity (const AngDir2 &);
    void set_force (const AngDir2 &);

    float get_mass () const;
    float get_radio () const;
    AngDir2 get_position () const;
    AngDir2 get_velocity () const;
    AngDir2 get_force () const;

    template <std::size_t N>
    friend class ParticleSource;
};
