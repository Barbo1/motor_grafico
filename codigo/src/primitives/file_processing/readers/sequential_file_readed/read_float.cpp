#include "../../../../../headers/primitives/file_processing.hpp"

float SequentialFileReader::read_float (int * error) {
  if (this->finished()) {
    *error = -1;
    return 0;
  } else *error = 0;

  std::string str = this->read_word();
  char * last;
  float ret = strtof(str.c_str(), &last);
  if (*last != '\0')
    *error = -2;
  return ret;
}

