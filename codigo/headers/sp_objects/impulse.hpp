#pragma once

#include "../concepts/primitives.hpp"

enum ImpulseType {
  ITHOLE,
  ITFAN
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
 */

/* Hole. */
template <ImpulseType IT, Function F, AngType AT>
class Impulse {
  private:
    Dir2 generated_force;

    Dir2 position;
    float radius;

  public:
    Impulse();
};
