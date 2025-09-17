#pragma once

/* A ZStack is a stack object that can store the objects that interacts with each other. 
 * It has the zpos member, that marks the position in the z axis. If two objects have the 
 * same zpos, they can collide. If both have differents zpos, the one with greater zpos 
 * may appear first in the screen. 
 * */

#include <cstdint>
#include <vector>
#include <map>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include "./primitives.hpp"
#include "./physical.hpp"

/* ---------------- */
/* Particle Source. */
/* ---------------- */

class ZStack {
  private:
    typedef struct {
      std::vector<AngDir2> sld_forces;
      std::vector<Physical*> sld_objects;
    } ZStackSlide;

    std::map<uint64_t, std::vector<ZStackSlide>> slide;
    uint64_t _time;  /* increased by one each time a frame is calculated. */

    std::vector<AngDir2*> _external_forces;

  public:
    ZStack (SDL_Color);
    ~ZStack ();

    void add_one_frc (const uint8_t &, AngDir2);
    void app_all_frc ();
    void del_sld_frc (const uint8_t &);
    void del_all_frc ();

    uint32_t add_sld_obj (const uint8_t &, const Physical&);
    void add_one_obj (const Physical&);
    void del_sld_obj (const uint8_t &);
    void del_all_obj ();

    void draw ();
};
