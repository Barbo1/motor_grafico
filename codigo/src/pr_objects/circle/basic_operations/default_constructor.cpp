#include "../../../../headers/pr_objects/circle.hpp"

Circle::Circle (): Physical(), radio(0) {
  this->texture = Visualizer<D2FIG>();
  this->glb = nullptr;
}
