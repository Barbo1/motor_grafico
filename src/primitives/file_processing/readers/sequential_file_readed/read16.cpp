#include "../../../../../headers/primitives/file_processing.hpp"

uint16_t SequentialFileReader::read16 () {
  uint16_t res = this->read8() | (this->read8() << 8);
  if constexpr (std::endian::native == std::endian::little)
    return std::byteswap(res);
  else
    return res;
}
