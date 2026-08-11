#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <string>
#include <string_view>
#include <cstdlib>

#include "../concepts/lights.hpp"

class Global {
  private: 
    SDL_Window* window;
    SDL_Renderer* render;
    SDL_Texture* global_render_texture;
    SDL_Texture* render_mask;
    
    Uint32 clock_ticks;

    /* configuration. */
    uint32_t c_height, c_width;
    SDL_Color c_bg_color;
    std::string c_window_name;

    /* singleton. */
    static Global* single;
    Global (std::string_view window_name, SDL_Color bg_color) noexcept;
    
    SDL_Color darkness_color;
    
  public:

    static Global* create (std::string_view window_name, SDL_Color bg_color) noexcept;
    ~Global ();

    /* Render functions. */
    void begin_render ();
    void end_render ();

    /* Internal information getters. */
    uint32_t get_height ();
    uint32_t get_width ();

    /* Wrapped information getters. */
    SDL_Window* get_window ();
    SDL_Renderer* get_render ();

    /* General getters. */
    float get_time ();
    uint32_t get_ticks ();
    uint32_t get_random ();
    float get_random_f01 ();

    /* Printing options. */
    void set_ambient_light (float per);
    void apply_mask (const ViewMask&);
};
