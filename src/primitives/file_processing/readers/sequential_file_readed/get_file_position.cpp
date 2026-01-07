#include "../../../../../headers/primitives/file_processing.hpp"

std::size_t SequentialFileReader::get_file_position () {
  return this->curr_file_pos;
}
