#include "../../../../../headers/primitives/file_processing.hpp"
#include "../../../../../headers/primitives/operations.hpp"

uint64_t SequentialFileReader::read64 () {
  return bytou64(
    read8(), read8(), read8(), read8(),
    read8(), read8(), read8(), read8()
  );
}
