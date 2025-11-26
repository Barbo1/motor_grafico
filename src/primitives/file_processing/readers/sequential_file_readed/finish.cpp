#include "../../../../../headers/primitives/file_processing.hpp"

void SequentialFileReader::finish () {
  this->is_lim = true;
  this->lim = this->pos;
}
