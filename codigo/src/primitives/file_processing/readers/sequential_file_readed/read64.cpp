#include "../../../../../headers/primitives/file_processing.hpp"
#include <cstdint>
#include <bit>

uint64_t SequentialFileReader::read64 () {
  uint64_t res = this->read8() | 
    (this->read8() << 8) | 
    (this->read8() << 16) | 
    (this->read8() << 24) | 
    ((uint64_t)this->read8() << 32) | 
    ((uint64_t)this->read8() << 40) | 
    ((uint64_t)this->read8() << 48) | 
    ((uint64_t)this->read8() << 56);
  if constexpr (std::endian::native == std::endian::little)
    return std::byteswap(res);
  else
    return res;
}
