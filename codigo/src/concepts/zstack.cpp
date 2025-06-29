#pragma once
#include "../../headers/concepts/zstack.hpp"
#include "../../headers/concepts/texture.hpp"

void draw (SDL_Renderer *) {}

void ZStackable::set_deletable (const bool & del) {
  this->deletable = del;
}

bool ZStackable::get_deletable () {
  return this->deletable;
}

void ZStackable::set_modified (const bool & mod) {
  this->modified = mod;
}

bool ZStackable::get_modified() {
  return this->modified;
}

void ZStackable::set_inherit (const bool & inh) {
  this->inherit = inh;
}

bool ZStackable::get_inherit () {
  return this->inherit;
}
