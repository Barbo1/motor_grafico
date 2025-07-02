#pragma once

#include "../../headers/pr_objects/square.hpp"
#include <utility>

Square::Square () : Physical (), Texture() {
  this->position = AngularDirection {0, 0, 0};
  this->height = 0;
  this->width = 0;
}

Square::Square(
  SDL_Renderer* render, uint32_t height, uint32_t width, AngularDirection center, SDL_Color color, 
  float density, float elasticity, float friction, bool movible, bool colidable
): Physical (center, density, height * width, elasticity, friction, movible, colidable), 
Texture(square(render, height, width, color)) {
  this->position = center;
  this->height = height;
  this->width = width;
}

Square::Square (const Square & square): Physical(square), Texture(square) {
  this->position = square.position;
  this->height = square.height;
  this->width = square.width;
}

Square::Square (Square && square): Physical(square), Texture(square) {
  this->position = std::exchange (square.position, {0,0,0});
  this->height = std::exchange (square.height, 0);
  this->width = std::exchange (square.width, 0);
}

Square & Square::operator= (const Square & square) {
  this->Physical::operator=(square);
  this->Texture::operator=(square);
  this->position = square.position;
  this->height = square.height;
  this->width = square.width;
  return *this;
}

Square & Square::operator= (Square && square) {
  this->Physical::operator=(square);
  this->Texture::operator=(square);
  this->position = std::exchange (square.position, {0,0,0});
  this->height = std::exchange (square.height, 0);
  this->width = std::exchange (square.width, 0);
  return *this;
}

void Square::draw (SDL_Renderer * render) {
  if (this->height != 0 && this->width != 0) {
    this->Texture::draw (render, {this->position.x, this->position.y, 0});
  }
}
