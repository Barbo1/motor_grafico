#include "../../../../headers/pr_objects/square.hpp"

Square::Square () : Physical () {
  this->position = AngDir2 {0.f, 0.f, 0.f};
  this->texture = Visualizer<D2FIG>();
  this->height = 0.f;
  this->width = 0.f;
  this->glb = nullptr;
}
