#include "../../../../../headers/primitives/file_processing.hpp"

bool SequentialFileReader::spare(std::size_t n) {
  return this->lim_file_pos - this->curr_file_pos - n > 0;
}
