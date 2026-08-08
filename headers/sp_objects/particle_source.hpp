#pragma once

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"
#include "../concepts/visualizer.hpp"
#include "../pr_objects/particle.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/nedge.hpp"
#include "../primitives/types_definition.hpp"
#include "../concepts/physical.hpp"

#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <utility>

const float PARTICLE_MASS = 100.f;

template <ParSrcType P, std::size_t N, Function F, Function T>
class ParticleSource {
  private:
    Global* glb;

    /* Particle infromation. */
    AngDir2 init_position;
    float velocity_coef;
    float force_coef;
    float init_scale;
    std::pair<float, float> change_angle;
    Visualizer<D2FIG> texture;

    /* General information. */
    uint32_t ticks_to_live;
    uint32_t ticks_init_need;
    uint32_t many_particles;
    uint32_t turned;

    /* Array of particles. */
    struct particle_data {
      AngDir2 force;
      uint32_t ticks;
    };
    std::array<std::pair<Particle, particle_data>, N> particles;

    /* helpers. */
    void particle_setter(Particle& particle);

  public:
    ParticleSource() = default;
    ParticleSource (
      Global* glb,
      const AngDir2& position, 
      const std::pair<float, float>& change_angle, 
      const Visualizer<D2FIG>& texture, 
      float radio, 
      float force_coef = 0.1f, 
      float velocity_coef = 1.f, 
      uint32_t ticks_to_live = 1200, 
      float init_scale = 0.f
    ) noexcept;
   
    void set_external_force (const AngDir2 &);
    void add_external_force (const AngDir2 &);
    void calculate_movement (const AngDir2 &);

    void up ();
    void down ();

    bool test_collition (Circle&);
    bool test_collition (Square&);
    bool test_collition (NEdge&);

    void correct_collition (Square&);
    void correct_collition (Circle&);
    void correct_collition (NEdge&);

    void resolve_collition (Square&);
    void resolve_collition (Circle&);
    void resolve_collition (NEdge&);

    void draw ();

    template <ImpulseType IT, UnitType UT, Function FT>
    friend class Impulse;
};

template <std::size_t N, Function F, Function T>
class ParticleSource<PS_EXPLOSION, N, F, T> {
  private:
    Global* glb;

    /* Array of particles. */
    struct particle_data {
      uint32_t ticks;
      AngDir2 force;
    };
    std::array<std::pair<Particle, particle_data>, N> particles;

    /* Particle infromation. */
    Visualizer<D2FIG> texture;
    std::pair<float, float> change_angle;
    AngDir2 init_position;
    float velocity_coef;

    /* General information. */
    uint32_t ticks_to_live;
    bool living;

    /* helpers. */
    void particle_setter(Particle& particle);

  public:
    ParticleSource() = default;
    ParticleSource (
      Global* glb,
      const AngDir2& position, 
      const std::pair<float, float>& change_angle, 
      const Visualizer<D2FIG>& texture, 
      float radio, 
      float velocity_coef = 1.f, 
      uint32_t ticks_to_live = 1200
    ) noexcept;
   
    void set_position (const AngDir2 &);
    void set_external_force (const AngDir2 &);
    void add_external_force (const AngDir2 &);
    void calculate_movement (const AngDir2 &);
    void burst ();
    bool bursting () const;

    bool test_collition (Circle&);
    bool test_collition (Square&);
    bool test_collition (NEdge&);

    void correct_collition (Square&);
    void correct_collition (Circle&);
    void correct_collition (NEdge&);

    void resolve_collition (Square&);
    void resolve_collition (Circle&);
    void resolve_collition (NEdge&);

    void draw ();

    template <ImpulseType IT, UnitType UT, Function FT>
    friend class Impulse;
};


/* 
 * ----------------------------------------
 * |    Implementation of Smooth source    |
 * -----------------------------------------
 */

template<ParSrcType P, std::size_t N, Function F, Function T>
void ParticleSource<P, N, F, T>::particle_setter(Particle& particle) {
  float coef = this->glb->get_random_f01 ();
  coef = this->change_angle.first * (1 - coef) + this->change_angle.second * coef;
  AngDir2 new_dir = AngDir2 (std::cos(coef), std::sin(coef), 0);

  particle.position.store(this->init_position);
  particle.velocity.store(new_dir * this->velocity_coef);
  particle.force.store(new_dir.percan() * this->force_coef);
}

template<ParSrcType P, std::size_t N, Function F, Function T>
ParticleSource<P, N, F, T>::ParticleSource(
  Global* glb, 
  const AngDir2& position, 
  const std::pair<float, float>& change_angle, 
  const Visualizer<D2FIG>& texture, 
  float radio, 
  float force_coef, 
  float velocity_coef, 
  uint32_t ticks_to_live,
  float init_scale
) noexcept : 
  glb(glb),
  init_position(position),
  velocity_coef(velocity_coef),
  force_coef(force_coef),
  init_scale(init_scale),
  change_angle(change_angle),
  texture(texture),
  ticks_to_live(ticks_to_live)
{ 
  for (uint32_t i = 0; i < N; i++) {
    this->particles[i] = std::pair<Particle, particle_data> (
      Particle (init_position, radio, PARTICLE_MASS), particle_data {AngDir2(), this->ticks_to_live + 1}
    );
  }
}

template<ParSrcType P, std::size_t N, Function F, Function T>
void ParticleSource<P, N, F, T>::set_external_force (const AngDir2 & force) {
  for (uint32_t i = 0; i < this->many_particles; i++) {
    auto& [particle, data] = this->particles[i];
    data.force = force;
  }
}

template<ParSrcType P, std::size_t N, Function F, Function T>
void ParticleSource<P, N, F, T>::calculate_movement (const AngDir2 & external_force) {
  uint32_t ticks = this->glb->get_ticks() + 1;
  float draw_coef = (this->glb->get_time() + 1.f) * DRAW_RATE;

  if (this->turned) {
    if (this->many_particles < N) {
      for (uint32_t i = 0; i < this->many_particles; i++) {
        auto& [particle, data] = this->particles[i];
        data.ticks += ticks;

        Dir2 vel = particle.velocity;
        Dir2 pos = particle.position;
        Dir2 new_vel = 
          (AngDir2(particle.force) + external_force + std::exchange(data.force, AngDir2 ()))
          .madd(draw_coef * 2.f / particle.mass, vel);
        particle.velocity.store(new_vel);
        particle.position.store(new_vel.madd(draw_coef, pos));
      }

      this->ticks_init_need += ticks;
      if (this->ticks_init_need * N >= this->ticks_to_live) {
        this->ticks_init_need = 0;
        this->particles[this->many_particles].second.ticks = 0;
        this->particle_setter (this->particles[this->many_particles++].first);
      }
    } else {
      for (auto& [particle, data]: this->particles) {
        if (data.ticks <= this->ticks_to_live) {
          data.ticks += ticks;

          Dir2 vel = particle.velocity;
          Dir2 pos = particle.position;
          Dir2 new_vel = 
            (AngDir2(particle.force) + external_force + std::exchange(data.force, AngDir2 ()))
            .madd(draw_coef * 2.f / particle.mass, vel);
          particle.velocity.store(new_vel);
          particle.position.store(new_vel.madd(draw_coef, pos));
        } else {
          data.ticks = 0;
          this->particle_setter (particle);
        }
      }
    }
  } else {
    for (auto& [particle, data]: this->particles) {
      if (data.ticks <= this->ticks_to_live) {
        data.ticks += ticks;

        Dir2 vel = particle.velocity;
        Dir2 pos = particle.position;
        Dir2 new_vel = 
          (AngDir2(particle.force) + external_force + std::exchange(data.force, AngDir2 ()))
          .madd(draw_coef * 2.f / particle.mass, vel);
        particle.velocity.store(new_vel);
        particle.position.store(new_vel.madd(draw_coef, pos));
      }
    }
  }
}

template<ParSrcType P, std::size_t N, Function F, Function T>
void ParticleSource<P, N, F, T>::up () {
  particle_setter(this->particles[0].first); 
  this->ticks_init_need = 0;
  this->many_particles = 1;
  this->turned = 1;
}

template<ParSrcType P, std::size_t N, Function F, Function T>
void ParticleSource<P, N, F, T>::down () {
  this->turned = 0;
  this->many_particles = 0;
}

template<ParSrcType P, std::size_t N, Function F, Function T>
void ParticleSource<P, N, F, T>::draw () {
  for (uint32_t i = 0; i < N; i++) {
    if (this->turned || this->particles[i].second.ticks <= this->ticks_to_live) {
      float scale = (float)this->particles[i].second.ticks / (float)this->ticks_to_live;

      /* applying alpha function. */
      if constexpr (F == FT_LINEAR) {
        this->texture.set_alpha(static_cast<Uint8>(255.f * scale));

      } else if constexpr (F == FT_QUADRATIC) {
        this->texture.set_alpha(static_cast<Uint8>(255.f * scale * scale));

      } else if constexpr (F == FT_CUBIC) {
        this->texture.set_alpha(static_cast<Uint8>(255.f * scale * scale * scale));
      
      } else if constexpr (F == FT_SINUSOIDAL) {
        float aux = std::sin(M_PI * scale);
        this->texture.set_alpha(static_cast<Uint8>(255.f * aux * aux * aux * aux));

      } else if constexpr (F == FT_LINEAR_N) {
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * scale));

      } else if constexpr (F == FT_QUADRATIC_N) {
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * scale * scale));

      } else if constexpr (F == FT_CUBIC_N) {
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * scale * scale * scale));

      } else if constexpr (F == FT_SINUSOIDAL_N) {
        float aux = std::sin(M_PI * scale);
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * aux * aux * aux * aux));
      }

      /* applying scale function. */
      if constexpr (T == FT_CONSTANT) {
        scale = 1.0f;
      } else if constexpr (T == FT_QUADRATIC) {
        scale *= scale;
      } else if constexpr (T == FT_CUBIC) {
        scale *= scale * scale;
      } else if constexpr (T == FT_SINUSOIDAL) {
        scale = std::sin(M_PI * scale);
        scale *= scale * scale * scale;
      } else if constexpr (T == FT_LINEAR_N) {
        scale = 1.0f - scale;
      } else if constexpr (T == FT_QUADRATIC_N) {
        scale = 1.0f - scale * scale;
      } else if constexpr (T == FT_CUBIC_N) {
        scale = 1.0f - scale * scale * scale;
      } else if constexpr (T == FT_SINUSOIDAL_N) {
        scale = std::sin(M_PI * scale);
        scale = 1.0f - scale * scale * scale * scale;
      }
     
      this->texture.draw (this->glb, this->particles[i].first.position, scale + this->init_scale);
    }
  }
}


/* 
 * --------------------------------------------
 * |    Implementation of Explosion source    |
 * --------------------------------------------
 */

template<std::size_t N, Function F, Function T>
void ParticleSource<PS_EXPLOSION, N, F, T>::particle_setter(Particle& particle) {
  float coef = this->glb->get_random_f01 ();
  coef = this->change_angle.first * (1 - coef) + this->change_angle.second * coef;
  AngDir2 new_dir = AngDir2 (std::cos(coef), std::sin(coef), 0);
  AngDir2 new_vel = new_dir * (this->glb->get_random_f01() * this->velocity_coef);

  particle.position.store (this->init_position);
  particle.velocity.store (new_vel);
}

template<std::size_t N, Function F, Function T>
ParticleSource<PS_EXPLOSION, N, F, T>::ParticleSource(
  Global* glb, 
  const AngDir2& position, 
  const std::pair<float, float>& change_angle, 
  const Visualizer<D2FIG>& texture, 
  float radio, 
  float velocity_coef, 
  uint32_t ticks_to_live
) noexcept :
  glb(glb),
  texture(texture),
  change_angle(change_angle),
  init_position(position),
  velocity_coef(velocity_coef),
  ticks_to_live(ticks_to_live),
  living(false)
{ 
  for (uint32_t i = 0; i < N; i++) {
    this->particles[i] = std::pair<Particle, particle_data> (
      Particle (
        init_position, 
        radio, 
        PARTICLE_MASS
      ), 
      particle_data {
        .ticks = static_cast<uint32_t>(ticks_to_live * 0.01f * glb->get_random_f01()), 
        .force = AngDir2()
      }
    );
  }
}

template<std::size_t N, Function F, Function T>
void ParticleSource<PS_EXPLOSION, N, F, T>::set_position (const AngDir2 & pos) {
  this->init_position = pos;
}

template<std::size_t N, Function F, Function T>
void ParticleSource<PS_EXPLOSION, N, F, T>::set_external_force (const AngDir2 & force) {
  for (auto& [_, data]: this->particles) {
    data.force = force;
  }
}

template<std::size_t N, Function F, Function T>
void ParticleSource<PS_EXPLOSION, N, F, T>::calculate_movement (const AngDir2 & external_force) {
  uint32_t ticks = this->glb->get_ticks() + 1;
  float draw_coef = (this->glb->get_time() + 1.f) * DRAW_RATE;

  bool living_particle = false;
  for (auto& [particle, data]: this->particles) {
    if (data.ticks <= this->ticks_to_live) {
      living_particle = true;
      data.ticks += ticks;
        
      Dir2 vel = Dir2(particle.velocity);
      Dir2 pos = Dir2(particle.position);
      Dir2 part = AngDir2(particle.force) + external_force + std::exchange(data.force, AngDir2 ());
      Dir2 new_vel = part.madd(draw_coef * 2.f / particle.mass, vel);
      particle.velocity.store(new_vel);
      particle.position.store(new_vel.madd(draw_coef, pos));
    }
  }
  this->living = living_particle;
}

template<std::size_t N, Function F, Function T>
void ParticleSource<PS_EXPLOSION, N, F, T>::burst () {
  this->living = true;
  for (auto& [particle, data]: this->particles) {
    this->particle_setter(particle);
    data.ticks = 0;
  }
}

template<std::size_t N, Function F, Function T>
bool ParticleSource<PS_EXPLOSION, N, F, T>::bursting () const {
  return this->living;
}

template<std::size_t N, Function F, Function T>
void ParticleSource<PS_EXPLOSION, N, F, T>::draw () {
  for (auto& [particle, data]: this->particles) {
    if (data.ticks <= this->ticks_to_live) {
      float scale = (float)data.ticks / (float)this->ticks_to_live;

      /* applying alpha function. */
      if constexpr (F == FT_LINEAR) {
        this->texture.set_alpha(static_cast<Uint8>(255.f * scale));

      } else if constexpr (F == FT_QUADRATIC) {
        this->texture.set_alpha(static_cast<Uint8>(255.f * scale * scale));

      } else if constexpr (F == FT_CUBIC) {
        this->texture.set_alpha(static_cast<Uint8>(255.f * scale * scale * scale));
      
      } else if constexpr (F == FT_SINUSOIDAL) {
        float aux = std::sin(M_PI * scale);
        this->texture.set_alpha(static_cast<Uint8>(255.f * aux * aux * aux * aux));

      } else if constexpr (F == FT_LINEAR_N) {
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * scale));

      } else if constexpr (F == FT_QUADRATIC_N) {
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * scale * scale));

      } else if constexpr (F == FT_CUBIC_N) {
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * scale * scale * scale));

      } else if constexpr (F == FT_SINUSOIDAL_N) {
        float aux = std::sin(M_PI * scale);
        this->texture.set_alpha(255 - static_cast<Uint8>(255.f * aux * aux * aux * aux));
      }
      
      /* applying scale function. */
      if constexpr (T == FT_CONSTANT) {
        scale = 1.0f;
      } else if constexpr (T == FT_QUADRATIC) {
        scale *= scale;
      } else if constexpr (T == FT_CUBIC) {
        scale *= scale * scale;
      } else if constexpr (T == FT_SINUSOIDAL) {
        scale = std::sin(M_PI * scale);
        scale *= scale * scale * scale;
      } else if constexpr (T == FT_LINEAR_N) {
        scale = 1.0f - scale;
      } else if constexpr (T == FT_QUADRATIC_N) {
        scale = 1.0f - scale * scale;
      } else if constexpr (T == FT_CUBIC_N) {
        scale = 1.0f - scale * scale * scale;
      } else if constexpr (T == FT_SINUSOIDAL_N) {
        scale = std::sin(M_PI * scale);
        scale = 1.0f - scale * scale * scale * scale;
      }
     
      this->texture.draw (this->glb, particle.position, scale);
    }
  }
}
