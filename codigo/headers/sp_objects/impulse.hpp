#pragma once

#include "../primitives/global.hpp"
#include "../primitives/vectors.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/square.hpp"

enum ImpulseType {
  IT_HOLE,
  IT_FAN
};

/* Hole. */
/* only posible to: IT_HOLE, AT_NONE, FT_CONSTANT. */
template <ImpulseType IT, UnitType UT, Function F>
class Impulse {
  private:
    AngDir2 position;
    float generated_force_coef;
    float radio;
    Global* glb;

  public:
    Impulse(Global*, AngDir2, float, float);

    float get_radio();
    float get_gfcoef();
    AngDir2 get_position();

    void set_radio(float);
    void set_gfcoef(float);
    void set_position(AngDir2);
  
    float apply_coef (Square);
    float apply_coef (Circle);
    AngDir2 apply (Square);
    AngDir2 apply (Circle);
};

/* 
  posible to: 
    + IT_FAN, AT_VELOCITY, FT_LINEAR 
    + IT_FAN, AT_VELOCITY, FT_QUADRATIC 
    + IT_FAN, AT_VELOCITY, FT_CUBIC 
    + IT_FAN, AT_VELOCITY, FT_SINUSOIDAL
    + IT_FAN, AT_VELOCITY, FT_EXPONENTIAL
*/
template <ImpulseType IT, Function F>
class Impulse<IT, UT_POSITION, F> {
  private:
    AngDir2 position;
    float generated_force_coef;
    float radio;
    Global* glb;

  public:
    Impulse(Global*, AngDir2, float, float);

    float get_radio();
    float get_gfcoef();
    AngDir2 get_position();
    AngDir2 get_direction();

    void set_radio(float);
    void set_gfcoef(float);
    void set_position(AngDir2);
  
    float apply_coef (Square);
    float apply_coef (Circle);
    AngDir2 apply (Square);
    AngDir2 apply (Circle);
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

    float get_width();
    float get_height();
    AngDir2 get_position();
    AngDir2 get_force();

    void set_width(float);
    void set_height(float);
    void set_position(AngDir2);
    void set_force(AngDir2);
  
    AngDir2 apply (Square);
    AngDir2 apply (Circle);
};


enum FanImpDir {
  FID_UP,
  FID_DOWN,
  FID_LEFT,
  FID_RIGHT
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

    float get_width();
    float get_height();
    float get_gfcoef();
    AngDir2 get_position();
    AngDir2 get_direction();

    void set_width(float);
    void set_height(float);
    void set_gfcoef(float);
    void set_position(AngDir2);
    void set_direction(FanImpDir);
  
    float apply_coef (Square);
    float apply_coef (Circle);
    AngDir2 apply (Square);
    AngDir2 apply (Circle);
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

    float get_width();
    float get_height();
    float get_gfcoef();
    AngDir2 get_position();

    void set_width(float);
    void set_height(float);
    void set_gfcoef(float);
    void set_position(AngDir2);
  
    float apply_coef (Square);
    float apply_coef (Circle);
    AngDir2 apply (Square);
    AngDir2 apply (Circle);
};
