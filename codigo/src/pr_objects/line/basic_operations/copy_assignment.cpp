#include "../../../../headers/pr_objects/line.hpp"

Line & Line::operator= (const Line & line) {
  this->slope = line.slope;
  this->deviation = line.deviation;
  return *this;
}
