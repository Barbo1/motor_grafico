#include "../../../../headers/pr_objects/line.hpp"

Line::Line (Dir2 p1, Dir2 p2) {
  this->slope = (p1.y - p2.y) / (p1.x - p2.x);
  this->deviation = p1.y - this->slope * p1.x;
}
