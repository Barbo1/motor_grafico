#include "../../../../headers/pr_objects/line.hpp"
    
Line::Line () noexcept {
  this->v = Dir2 (1.f, 0.f);
  this->p = Dir2 (0.f, 0.f);
};
