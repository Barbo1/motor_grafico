#include "../../../../../headers/primitives/file_processing.hpp"
#include <cstdint>

std::vector<uint8_t> SequentialFileReader::get_vector (std::size_t f) {
  std::vector<uint8_t> vec(f, 0);
  for (std::size_t i = 0; i < f; i++)
    vec[i] = this->read8();
  return vec;
}
