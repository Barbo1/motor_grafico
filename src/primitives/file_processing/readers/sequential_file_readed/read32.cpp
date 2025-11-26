#include "../../../../../headers/primitives/file_processing.hpp"

uint32_t SequentialFileReader::read32 () {
  uint32_t res = this->read8() | 
    (this->read8() << 8) | 
    (this->read8() << 16) | 
    (this->read8() << 24);
  if constexpr (std::endian::native == std::endian::little)
    return std::byteswap(res);
  else
    return res;
}
