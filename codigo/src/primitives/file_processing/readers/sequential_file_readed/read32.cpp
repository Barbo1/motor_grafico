#include "../../../../../headers/primitives/file_processing.hpp"
#include "../../../../../headers/primitives/operations.hpp"

uint32_t SequentialFileReader::read32 () {
  return bytou32(read8(), read8(), read8(), read8());
}
