#include "../../../../headers/pr_objects/square.hpp"

Square::Square () noexcept : Physical () {
  this->position = MemDir2();
  this->texture = Visualizer<D2FIG>();
  this->dims = MemDir2();
  this->glb = nullptr;
}
