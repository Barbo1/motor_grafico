#include "../../../../headers/pr_objects/line.hpp"

Line::Line (const Line & line) {
  this->slope = line.slope;
  this->intersection = line.intersection;
}
