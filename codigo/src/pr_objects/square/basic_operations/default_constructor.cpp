#include "../../../../headers/pr_objects/square.hpp"

Square::Square () : Physical () {
  this->position = AngDir2 {0, 0, 0};
  this->texture = Visualizer<D2FIG>();
  this->height = 0;
  this->width = 0;
}
