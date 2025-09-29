#include "../../../../headers/pr_objects/line.hpp"
#include <utility>

Line::Line (Line && line) {
  this->slope = std::exchange (line.slope, 0);
  this->intersection = std::exchange (line.intersection, 0);
}
