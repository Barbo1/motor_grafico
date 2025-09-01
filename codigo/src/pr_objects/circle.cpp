#include "../../headers/pr_objects/circle.hpp"

#include <cmath>
#include <utility>

Circle::Circle (): Physical(), radio(0) {
  this->texture = Visualizer<D2FIG>();
}

Circle::Circle (
  SDL_Renderer* render, uint32_t radio, AngDir2 center, SDL_Color color, float density, float elasticity, 
  float f_s, float f_k, bool movible, bool colidable
) : Physical (center, density, 2 * M_PI * radio, elasticity, f_s, f_k, movible, colidable), 
  radio (radio)
{
  this->texture = Visualizer<D2FIG>::circle(render, radio, color);
}

Circle::Circle (const Circle & circle) : Physical (circle) {
  this->radio = circle.radio;
  this->texture = circle.texture;
}

Circle::Circle (Circle && circle) : Physical (circle) {
  this->radio = std::exchange(circle.radio, 0);
  this->texture = std::exchange(circle.texture, Visualizer<D2FIG>());
}

Circle & Circle::operator= (const Circle & circle) {
  this->Physical::operator=(circle);
  this->texture = circle.texture;
  this->radio = circle.radio;
  return *this;
}

Circle & Circle::operator= (Circle && circle) {
  this->Physical::operator=(std::move(circle));
  this->radio = std::exchange(circle.radio, 0);
  this->texture = std::exchange(circle.texture, Visualizer<D2FIG>());
  return *this;
}

void Circle::set_radio (uint32_t radio) {
  this->radio = radio;
}

uint32_t Circle::get_radio () {
  return this->radio;
}

void Circle::draw (SDL_Renderer * render) {
  if (this->radio != 0) {
    this->texture.draw(render, this->position);
  }
}
