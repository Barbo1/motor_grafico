#include "../../../../../headers/primitives/file_processing.hpp"
#include "../../../../../headers/primitives/operations.hpp"

uint16_t SequentialFileReader::read16 () {
  return bytou16(read8(), read8());
}
