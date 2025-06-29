#pragma once

#include "../../headers/pr_objects/square.hpp"
#include <utility>

Square::Square () : Physical () {
  this->position = Direction {0, 0};
  this->color = SDL_Color {0, 0, 0, 0};
  this->_edges = std::vector<Direction>();
  this->_rect = SDL_Rect {0, 0, 0, 0};
}

Square::Square (uint32_t height, uint32_t width, Direction center, SDL_Color color) {
  this->position = center;
  this->color = color;
  this->_edges = std::vector<Direction>();
  this->_rect = SDL_Rect {(int)center.x, (int)center.y, (int)height, (int)width};
}

Square::Square (const Square & square) {
  this->position = square.position;
  this->color = square.color;
  this->_edges = std::vector<Direction>(square._edges);
  this->_rect = square._rect;
}

Square::Square (Square && square) {
  this->position = square.position;
  this->color = square.color;
  this->_edges = std::vector<Direction>(square._edges);
  this->_rect = std::exchange(square._rect, SDL_Rect {0, 0, 0, 0});
}

Square & Square::operator= (const Square & square) {
  this->position = square.position;
  this->color = square.color;
  this->_edges = std::vector<Direction>(square._edges);
  this->_rect = square._rect;
  return *this;
}

Square & Square::operator= (Square && square) {
  this->position = square.position;
  this->color = square.color;
  this->_edges = std::vector<Direction>(square._edges);
  this->_rect = std::exchange(square._rect, SDL_Rect {0, 0, 0, 0});
  return *this;
}

void Square::draw (SDL_Renderer * render) {
  this->_rect.x = this->position.x;
  this->_rect.y = this->position.y;

  SDL_SetRenderDrawColor(render, this->color.r, this->color.g, this->color.b, this->color.a);
  SDL_RenderFillRect(render, &this->_rect);
}
