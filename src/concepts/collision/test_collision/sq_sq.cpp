#include "../../../../headers/pr_objects/square.hpp"

bool test_collision (const Square& sq1, const Square& sq2) {
  Dir2 diff = (Dir2(sq1.position) - Dir2(sq2.position)).abs();
  Dir2 dims_sum = Dir2(sq1.dims) + Dir2(sq2.dims);
  return diff < dims_sum;
}
