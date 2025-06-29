#pragma once

/* A ZStack is a stack object that can store the objects that interacts with each other. 
 * It has the zpos member, that marks the position in the z axis. If two objects have the 
 * same zpos, they can collide. If both have differents zpos, the one with greater zpos 
 * may appear first in the screen. 
 * */

#include <vector>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include "./primitives.hpp"

/* ---------------- */
/* Particle Source. */
/* ---------------- */

class ZStackable {
  private: 
    bool deletable; /* a ZStackable object can decide if he can be destoryed by ZStack. */
    bool modified; /* true if the ZStackable object has changed in velocity or forces. */
    bool inherit; /* if false, the object will never inherit the forces in the ZStack. */

  public:
    virtual void draw (SDL_Renderer *) = 0;

    void set_deletable (const bool &);
    bool get_deletable ();

    void set_modified (const bool &);
    bool get_modified();

    void set_inherit (const bool &);
    bool get_inherit ();
};

class Window {
  private:
    typedef struct {
      std::vector<AngularDirection> sld_forces;
      std::vector<ZStackable*> sld_objects;
    } WindowElem;

    std::vector<WindowElem> _slides;
    std::vector<uint32_t> _indexes;
    uint64_t _time;  /* increased by one each time a frame is calculated. */
    std::vector<AngularDirection*> _external_forces;

  public:
    Window (SDL_Color);

    void add_one_frc (const uint8_t &, AngularDirection);
    void app_all_frc ();
    void del_sld_frc (const uint8_t &);
    void del_all_frc ();

    uint32_t add_sld_obj (const uint8_t &, const ZStackable&);
    void add_one_obj (const ZStackable&);
    void del_sld_obj (const uint8_t &);
    void del_all_obj ();

    void draw ();
    
    ~Window ();
};
