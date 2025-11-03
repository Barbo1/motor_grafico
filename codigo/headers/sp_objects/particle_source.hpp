#pragma once

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"
#include "../concepts/visualizer.hpp"
#include "../pr_objects/particle.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/nedge.hpp"

#include <cstdint>
#include <array>
#include <SDL2/SDL_pixels.h>

template <std::size_t N>
class ParticleSource {
  private:
    /* Particle physical infromation. */
    AngDir2 init_position;
    float velocity_coef;
    float force_coef;
    std::pair<float, float> change_angle;

    /* Information about particles life. */
    uint32_t spawn_ratio;
    uint32_t frames_since_new_particule;
    uint32_t init_blur;

    /* Array of particles. */
    struct particle_data {
      uint32_t blur;
      bool valid;
    };
    std::array<std::pair<Particle, particle_data>, N> particles;

    /* Particle's texture. */
    Visualizer<D2FIG> texture;

    Global* glb;

  public:
    ParticleSource<N>(
      Global* glb, const AngDir2 & position, const AngDir2 & velocity, 
      const std::pair<float, float> & change_angle, const Visualizer<D2FIG>& texture, 
      float force_coef = 0.1f, float velocity_coef = 1.f, uint32_t spawn_ratio = 1200, 
      uint32_t blur = 5000
    );
    
    /* Calcula siguiente movimiento del las particular. Destruye particulas 
     * si llegan a la condicion, crea en caso de que se pueda e increemnta 
     * spawn_ratio. 
     * */
    void calculate_movement (const AngDir2 & external_force);

    bool test_collition(Physical&);
    bool test_collition(Circle&);
    bool test_collition(Square&);
    bool test_collition(NEdge&);

    void correct_collition (Physical&);
    void correct_collition (Square&);
    void correct_collition (Circle&);
    void correct_collition (NEdge&);

    void resolve_collition (Physical&);
    void resolve_collition (Square&);
    void resolve_collition (Circle&);
    void resolve_collition (NEdge&);

    void draw (SDL_Renderer * render);
};
