#include "../../../../headers/pr_objects/line.hpp"
    
Line::Line () noexcept {
  this->v = MemDir2(1.f, 0.f);
  this->p = MemDir2(0.f, 0.f);
};
