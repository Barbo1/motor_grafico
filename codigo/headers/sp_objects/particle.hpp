#pragma once

#include <cstdint>
#include <array>
#include <SDL2/SDL_pixels.h>

#include "../primitives/vectors.hpp"
#include "../concepts/visualizer.hpp"

struct Particle {
  AngDir2 position;
  AngDir2 velocity;
  
  float coef; /*  */
  float blur; /* if blur less becomes 0, the particle is destroyed. */
};

/* ---------------- */
/* Particle Source. */
/* ---------------- */

template <std::size_t N>
class ParticleSource {
  private:
    AngDir2 init_position;
    AngDir2 init_velocity;

    /* Particles per frames. */
    uint32_t spawn_ratio;
    uint32_t frames_since_new_particule;

    std::pair<float, float> change_angle;
    float force_coef;

    Visualizer<D2FIG> texture;
    std::array<Particle, N> particles;

  public:
    ParticleSource<N>(
      SDL_Renderer * render, const AngDir2 & position, const AngDir2 & velocity, 
      const std::pair<float, float> & change_angle, float force_coef, 
      Visualizer<D2FIG> texture, uint32_t spawn_ratio = 12
    );
    
    /* calcula siguiente movimiento del las particular. Destruye particulas si llegan a la 
     * condicion, crea en caso de que se pueda e increemnta spawn_ratio. 
     * */
    void calculate_movement(const AngDir2 & external_force);

    void draw (SDL_Renderer * render);
};
