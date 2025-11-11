#pragma once

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"
#include "../concepts/visualizer.hpp"
#include "../pr_objects/particle.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/nedge.hpp"
#include "../primitives/types_definition.hpp"

#include <SDL2/SDL.h>
#include <array>
#include <utility>

const float PARTICLE_MASS = 100.f;

template <std::size_t N, Function F>
class ParticleSource {
  private:
    Global* glb;

    /* Particle infromation. */
    AngDir2 init_position;
    float velocity_coef;
    float force_coef;
    std::pair<float, float> change_angle;
    Visualizer<D2FIG> texture;
    float radio;

    /* General information. */
    uint32_t ticks_to_live;
    uint32_t ticks_init_need;
    uint32_t many_particles;

    /* Array of particles. */
    struct particle_data {
      uint32_t ticks;
      AngDir2 force;
    };
    std::array<std::pair<Particle, particle_data>, N> particles;

    void particle_setter(Particle& particle);

  public:
    ParticleSource (
      Global* glb, 
      const AngDir2& position, 
      const std::pair<float, float>& change_angle, 
      const Visualizer<D2FIG>& texture, 
      float radio, 
      float force_coef = 0.1f, 
      float velocity_coef = 1.f, 
      uint32_t ticks_to_live = 1200
    ) noexcept;
   
    void set_external_force (const AngDir2 &);
    void add_external_force (const AngDir2 &);
    void calculate_movement (const AngDir2 &);

    bool test_collition (Physical&);
    bool test_collition (Circle&);
    bool test_collition (Square&);
    bool test_collition (NEdge&);

    void correct_collition (Physical&);
    void correct_collition (Square&);
    void correct_collition (Circle&);
    void correct_collition (NEdge&);

    void resolve_collition (Physical&);
    void resolve_collition (Square&);
    void resolve_collition (Circle&);
    void resolve_collition (NEdge&);

    void draw ();

    template <ImpulseType IT, UnitType UT, Function FT>
    friend class Impulse;
};

template<std::size_t N, Function F>
void ParticleSource<N, F>::particle_setter(Particle& particle) {
  float coef = this->glb->get_random_f01 ();
  coef = this->change_angle.first * (1 - coef) + this->change_angle.second * coef;
  AngDir2 new_dir = AngDir2 (std::sin(coef), std::cos(coef), 0);

  particle.set_position (this->init_position);
  particle.set_velocity (new_dir * this->velocity_coef);
  particle.set_force (new_dir.percan() * this->force_coef);
}

template<std::size_t N, Function F>
ParticleSource<N, F>::ParticleSource(
    Global* glb, 
    const AngDir2& position, 
    const std::pair<float, float>& change_angle, 
    const Visualizer<D2FIG>& texture, 
    float radio, 
    float force_coef, 
    float velocity_coef, 
    uint32_t ticks_to_live
) noexcept : 
  glb(glb),
  init_position(position),
  velocity_coef(velocity_coef),
  force_coef(force_coef),
  change_angle(change_angle),
  texture(texture),
  radio(radio),
  ticks_to_live(ticks_to_live),
  ticks_init_need(0),
  many_particles(1)
{ 
  for (uint32_t i = 0; i < N; i++) {
    this->particles[i] = std::pair<Particle, particle_data> (
      Particle (glb, radio, init_position, PARTICLE_MASS), particle_data {0, AngDir2()}
    );
  }
  particle_setter(this->particles[0].first); 
}

template<std::size_t N, Function F>
void ParticleSource<N, F>::set_external_force (const AngDir2 & force) {
  for (uint32_t i = 0; i < this->many_particles; i++) {
    auto& [particle, data] = this->particles[i];
    data.force = force;
  }
}

template<std::size_t N, Function F>
void ParticleSource<N, F>::calculate_movement (const AngDir2 & external_force) {
  uint32_t ticks = this->glb->get_ticks();
  float draw_coef = this->glb->get_time() * DRAW_RATE;

  if (this->many_particles < N) {
    for (uint32_t i = 0; i < this->many_particles; i++) {
      auto& [particle, data] = this->particles[i];
      data.ticks += ticks;
  
      particle.add_velocity (
        (particle.get_force () + external_force + std::exchange(data.force, AngDir2 ())) * 
        draw_coef * (20000.f / particle.get_mass())
      );
      particle.add_position (particle.get_velocity () * draw_coef);
    }

    this->ticks_init_need += ticks;
    if (this->ticks_init_need * N >= this->ticks_to_live) {
      this->ticks_init_need = 0;
      this->particle_setter (this->particles[this->many_particles++].first);
    }
  } else [[likely]] {
    for (auto& [particle, data]: this->particles) {
      data.ticks += ticks;

      if (data.ticks <= this->ticks_to_live) [[likely]] {
        particle.add_velocity (
          (particle.get_force () + external_force + std::exchange(data.force, AngDir2 ())) * 
          draw_coef * (20000.f / particle.get_mass())
        );
        particle.add_position (particle.get_velocity () * draw_coef);
      } else {
        data.ticks = 0;
        this->particle_setter (particle);
      }
    }
  }
}

template<std::size_t N, Function F>
void ParticleSource<N, F>::draw () {
  for (uint32_t i = 0; i < this->many_particles; i++) {
    if constexpr (F == FT_CONSTANT) {
      this->texture.draw (this->glb, this->particles[i].first.get_position());

    } else if constexpr (F == FT_LINEAR) {
      float aux = (float)this->particles[i].second.ticks / (float)this->ticks_to_live;
      this->texture.set_alpha(255 - static_cast<Uint8>(255.f * aux));
      this->texture.draw (this->glb, this->particles[i].first.get_position());

    } else if constexpr (F == FT_QUADRATIC) {
      float aux = (float)this->particles[i].second.ticks / (float)this->ticks_to_live;
      this->texture.set_alpha(255 - static_cast<Uint8>(255.f * aux * aux));
      this->texture.draw (this->glb, this->particles[i].first.get_position());

    } else if constexpr (F == FT_CUBIC) {
      float aux = (float)this->particles[i].second.ticks / (float)this->ticks_to_live;
      this->texture.set_alpha(255 - static_cast<Uint8>(255.f * aux * aux * aux));
      this->texture.draw (this->glb, this->particles[i].first.get_position());

    } else if constexpr (F == FT_SINUSOIDAL) {
      float aux = std::sin(M_PI * this->particles[i].second.ticks / this->ticks_to_live);
      this->texture.set_alpha(static_cast<Uint8>(255.f * aux * aux * aux * aux));
      this->texture.draw (this->glb, this->particles[i].first.get_position());
    }
  }
}
