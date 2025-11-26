#include "../../../../../headers/primitives/file_processing.hpp"

bool SequentialFileReader::finished () {
  return this->is_lim && this->pos >= this->lim;
}
