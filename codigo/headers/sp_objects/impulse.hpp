#pragma once

#include "../primitives/global.hpp"
#include "../primitives/vectors.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/particle.hpp"
#include "../sp_objects/particle_source.hpp"

#include <iostream>

/* Hole. */
/* 
  posible to: 
    + IT_HOLE, AT_VELOCITY, FT_CONSTANT
    + IT_HOLE, AT_VELOCITY, FT_LINEAR 
    + IT_HOLE, AT_VELOCITY, FT_QUADRATIC 
    + IT_HOLE, AT_VELOCITY, FT_CUBIC 
    + IT_HOLE, AT_VELOCITY, FT_SINUSOIDAL
    + IT_HOLE, AT_VELOCITY, FT_EXPONENTIAL
*/
template <ImpulseType IT, UnitType UT, Function F>
class Impulse {
  private:
    AngDir2 position;
    float generated_force_coef;
    float radio;
    Global* glb;

  public:
    Impulse(Global*, AngDir2, float, float);

    [[nodiscard]] float get_radio();
    [[nodiscard]] float get_gfcoef();
    [[nodiscard]] AngDir2 get_position();
    [[nodiscard]] AngDir2 get_direction();

    void set_radio(float);
    void set_gfcoef(float);
    void set_position(AngDir2&);
  
    float apply_coef (Square&);
    float apply_coef (Circle&);
    float apply_coef (Particle&);
    AngDir2 apply (Square&);
    AngDir2 apply (Circle&);
    AngDir2 apply (Particle&);

    template<std::size_t N, Function FT> ParticleSource<N, FT>& apply (ParticleSource<N, FT>& ps) {
      for (auto& [particle, data] : ps.particles)
        data.force += this->apply (particle);
      return ps;
    }
};


/* Fan. */
/* only posible to: IT_FAN, AT_NONE, FT_CONSTANT. */
template <UnitType UT, Function F>
class Impulse<IT_FAN, UT, F> {
  private:
    AngDir2 position;
    AngDir2 force;
    AngDir2 dimension;
    Global* glb;

  public:
    Impulse (Global*, AngDir2, AngDir2, float, float);

    [[nodiscard]] float get_width();
    [[nodiscard]] float get_height();
    [[nodiscard]] AngDir2 get_position();
    [[nodiscard]] AngDir2 get_force();

    void set_width(float);
    void set_height(float);
    void set_position(AngDir2&);
    void set_force(AngDir2&);
  
    [[nodiscard]] AngDir2 apply (Square&);
    [[nodiscard]] AngDir2 apply (Circle&);
    [[nodiscard]] AngDir2 apply (Particle&);

    template<std::size_t N, Function FT> ParticleSource<N, FT>& apply (ParticleSource<N, FT>& ps) {
      for (auto& [particle, data] : ps.particles) {
        AngDir2 a = this->apply (particle);
        std::cout << "(" << a.x << ", " << a.y << ")" << std::endl;
        a = particle.get_position();
        std::cout << "(" << a.x << ", " << a.y << ")" << std::endl;
        a = particle.get_velocity();
        std::cout << "(" << a.x << ", " << a.y << ")" << std::endl;
        data.force += this->apply (particle);
      }
      return ps;
    }
};


/* 
  posible to: 
    + IT_FAN, AT_VELOCITY, FT_LINEAR 
    + IT_FAN, AT_VELOCITY, FT_QUADRATIC 
    + IT_FAN, AT_VELOCITY, FT_CUBIC 
    + IT_FAN, AT_VELOCITY, FT_SINUSOIDAL
    + IT_FAN, AT_VELOCITY, FT_EXPONENTIAL
*/
template <Function F>
class Impulse<IT_FAN, UT_POSITION, F> {
  private:
    AngDir2 position;
    AngDir2 dimension;
    AngDir2 direction;
    float generated_force_coef;
    float sign;
    Global* glb;

  public:
    Impulse (Global*, AngDir2, float, float, float, FanImpDir);

    [[nodiscard]] float get_width();
    [[nodiscard]] float get_height();
    [[nodiscard]] float get_gfcoef();
    [[nodiscard]] AngDir2 get_position();
    [[nodiscard]] AngDir2 get_direction();

    void set_width(float);
    void set_height(float);
    void set_gfcoef(float);
    void set_position(AngDir2&);
    void set_direction(FanImpDir);
  
    [[nodiscard]] float apply_coef (Square&);
    [[nodiscard]] float apply_coef (Circle&);
    [[nodiscard]] float apply_coef (Particle&);
    [[nodiscard]] AngDir2 apply (Square&);
    [[nodiscard]] AngDir2 apply (Circle&);
    [[nodiscard]] AngDir2 apply (Particle&);

    template<std::size_t N, Function FT> ParticleSource<N, FT>& apply (ParticleSource<N, FT>& ps) {
      for (auto& [particle, data]: ps.particles)
        data.force += this->apply (particle);
      return ps;
    }
};

/* 
  posible to: 
    + IT_FAN, AT_VELOCITY, FT_LINEAR 
    + IT_FAN, AT_VELOCITY, FT_QUADRATIC 
    + IT_FAN, AT_VELOCITY, FT_CUBIC 
    + IT_FAN, AT_VELOCITY, FT_EXPONENTIAL
*/
template <Function F>
class Impulse<IT_FAN, UT_VELOCITY, F> {
  private:
    AngDir2 position;
    AngDir2 dimension;
    float generated_force_coef;
    Global* glb;

  public:
    Impulse (Global*, AngDir2, float, float, float);

    [[nodiscard]] float get_width();
    [[nodiscard]] float get_height();
    [[nodiscard]] float get_gfcoef();
    [[nodiscard]] AngDir2 get_position();

    void set_width(float);
    void set_height(float);
    void set_gfcoef(float);
    void set_position(AngDir2&);
  
    [[nodiscard]] float apply_coef (Square&);
    [[nodiscard]] float apply_coef (Circle&);
    [[nodiscard]] float apply_coef (Particle&);
    [[nodiscard]] AngDir2 apply (Square&);
    [[nodiscard]] AngDir2 apply (Circle&);
    [[nodiscard]] AngDir2 apply (Particle&);

    template<std::size_t N, Function FT> ParticleSource<N, FT>& apply (ParticleSource<N, FT>& ps) {
      for (auto& [particle, data]: ps.particles)
        data.force += this->apply (particle);
      return ps;
    }
};
