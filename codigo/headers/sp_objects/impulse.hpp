#pragma once

#include "../primitives/global.hpp"
#include "../primitives/vectors.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/square.hpp"

enum ImpulseType {
  IT_HOLE,
  IT_FAN
};

/*
 ITHOLE ->
  1 -> F = FT_CONSTANT, AT = Any, 
      tiene posicion, radio y fuerza generada.
  2 -> F = {FT_LINEAR, FT_QUADRATIC, FT_CUBIC, FT_SINUSOIDAL}, AT = AT_POSIITON,
      tiene posicion, radio y coeficiente de fuerza generada.
  3 -> F = {FT_LINEAR, FT_QUADRATIC, FT_CUBIC, FT_SINUSOIDAL}, AT = AT_TIME,
      tiene posicion, radio, coeficiente de fuerza generada y cantidad de tiempo que paso.

 ITFAN ->
  1 -> F = FT_CONSTANT, AT = Any, 
      tiene posicion, alto, largo y fuerza generada.
  2 -> F = {FT_LINEAR, FT_QUADRATIC, FT_CUBIC, FT_SINUSOIDAL}, AT = AT_POSIITON,
      tiene posicion, alto, largo y coeficiente de fuerza generada.
  3 -> F = {FT_LINEAR, FT_QUADRATIC, FT_CUBIC, FT_SINUSOIDAL}, AT = AT_TIME,
      tiene posicion, alto, largo, coeficiente de fuerza generada y cantidad de tiempo que paso.
  4 -> F = {FT_LINEAR, FT_QUADRATIC, FT_CUBIC}, AT = AT_VELOCITY,
      tiene posicion, alto, largo y coeficiente de fuerza generada.

  En todos los casos, la funcion apply retorna la fuerza que se le debe aplicar al objeto
  evaluado. Si no se genera una colision, la fuerza retornada es (0,0).
 */

/* Hole. */
/* only posible to: IT_HOLE, AT_NONE, FT_CONSTANT. */
template <ImpulseType IT, AngType AT, Function F>
class Impulse {
  private:
    AngDir2 position;
    float generated_force_coef;
    float radio;
    Global* glb;

  public:
    Impulse(Global*, AngDir2, float, float);

    float get_radio();
    void set_radio(float);

    float get_gfcoef();
    void set_gfcoef(float);

    AngDir2 get_position();
    void set_position(AngDir2);
  
    AngDir2 apply(Square);
    AngDir2 apply(Circle);
};

/* 
  only posible to: IT_HOLE, AT_POSIITON, FT_LINEAR 
  only posible to: IT_HOLE, AT_POSIITON, FT_QUADRATIC 
  only posible to: IT_HOLE, AT_POSIITON, FT_CUBIC 
  only posible to: IT_HOLE, AT_POSIITON, FT_SINUSOIDAL 
  only posible to: IT_HOLE, AT_POSIITON, FT_EXPONENTIAL
*/
template <ImpulseType IT, Function F>
class Impulse<IT, AT_POSITION, F> {
  private:
    AngDir2 position;
    float generated_force_coef;
    float radio;
    Global* glb;

  public:
    Impulse(Global*, AngDir2, float, float);

    float get_radio();
    void set_radio(float);

    float get_gfcoef();
    void set_gfcoef(float);

    AngDir2 get_position();
    void set_position(AngDir2);
  
    AngDir2 apply(Square);
    AngDir2 apply(Circle);
};
