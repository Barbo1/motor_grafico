#pragma once

#include "../../headers/pr_objects/circle.hpp"

#include <cmath>
#include <utility>

Circle::Circle (): Physical(), Texture(), radio(0) {}

Circle::Circle (
  SDL_Renderer* render, uint32_t radio, AngularDirection center, SDL_Color color, float density, float elasticity, 
  float f_s, float f_k, bool movible, bool colidable
) : Physical (center, density, 2 * M_PI * radio, elasticity, f_s, f_k, movible, colidable), 
  Texture(circle(render, radio, color)),
  radio (radio)
{}

Circle::Circle (const Circle & circle) : Physical (circle), Texture (circle) {
  this->radio = circle.radio;
}

Circle::Circle (Circle && circle) : Physical (circle), Texture (circle) {
  this->radio = std::exchange(circle.radio, 0);
}

Circle & Circle::operator= (const Circle & circle) {
  this->Physical::operator=(circle);
  this->Texture::operator=(circle);
  this->radio = circle.radio;
  return *this;
}

Circle & Circle::operator= (Circle && circle) {
  this->Physical::operator=(std::move(circle));
  this->Texture::operator=(std::move(circle));
  this->radio = std::exchange(circle.radio, 0);
  return *this;
}

Circle::~Circle () {}

void Circle::set_radio (uint32_t radio) {
  this->radio = radio;
}

uint32_t Circle::get_radio () {
  return this->radio;
}

void Circle::draw (SDL_Renderer * render) {
  if (this->radio != 0) {
    this->Texture::draw(render, this->position);
  }
}
