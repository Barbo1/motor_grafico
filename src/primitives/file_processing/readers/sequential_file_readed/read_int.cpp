#include "../../../../../headers/primitives/file_processing.hpp"

uint64_t SequentialFileReader::read_int (int * error) {
  if (this->finished()) {
    *error = -1;
    return 0;
  } else *error = 0;

  std::string str = this->read_word();
  char * last;
  uint64_t ret = strtoull(str.c_str(), &last, 0);
  if (*last != '\0')
    *error = -2;
  return ret;
}
