#pragma once

#include "../../headers/pr_objects/square.hpp"
#include <utility>

Square::Square () : Physical () {
  this->position = AngDir2 {0, 0, 0};
  this->texture = Visualizer<D2FIG>();
  this->height = 0;
  this->width = 0;
}

Square::Square (
  SDL_Renderer* render, uint32_t height, uint32_t width, AngDir2 center, SDL_Color color, 
  float density, float elasticity, float f_s, float f_k, bool movible, bool colidable
): Physical (center, density, height * width, elasticity, f_s, f_k, movible, colidable) {
  this->texture = Visualizer<D2FIG>::square(render, height, width, color);
  this->position = center;
  this->height = height;
  this->width = width;
}

Square::Square (const Square & square): Physical(square) {
  this->position = square.position;
  this->texture = square.texture;
  this->height = square.height;
  this->width = square.width;
}

Square::Square (Square && square): Physical(square) {
  this->texture = std::exchange (square.texture, Visualizer<D2FIG>());
  this->position = std::exchange (square.position, {0,0,0});
  this->height = std::exchange (square.height, 0);
  this->width = std::exchange (square.width, 0);
}

Square & Square::operator= (const Square & square) {
  this->Physical::operator=(square);
  this->texture = square.texture;
  this->position = square.position;
  this->height = square.height;
  this->width = square.width;
  return *this;
}

Square & Square::operator= (Square && square) {
  this->Physical::operator=(square);
  this->texture = std::exchange (square.texture, Visualizer<D2FIG>());
  this->position = std::exchange (square.position, {0,0,0});
  this->height = std::exchange (square.height, 0);
  this->width = std::exchange (square.width, 0);
  return *this;
}

void Square::draw (SDL_Renderer * render) {
  if (this->height != 0 && this->width != 0) {
    this->texture.draw (render, {this->position.x, this->position.y, 0});
  }
}
