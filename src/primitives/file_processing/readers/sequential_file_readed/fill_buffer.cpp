#include "../../../../../headers/primitives/file_processing.hpp"
#include <cstdint>

void SequentialFileReader::fill_buffer (uint8_t* arr, std::size_t s, std::size_t f) {
  for (std::size_t i = s; i < f; i++)
    arr[i] = this->read8();
}
