#include "../../../../headers/pr_objects/line.hpp"
#include <utility>

Line::Line (Line && line) {
  this->slope = std::exchange (line.slope, 0);
  this->deviation = std::exchange (line.deviation, 0);
}
