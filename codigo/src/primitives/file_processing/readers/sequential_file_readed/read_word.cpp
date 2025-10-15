#include "../../../../../headers/primitives/file_processing.hpp"

std::string SequentialFileReader::read_word () {
  char let;
  std::string str = "";
  while((let = this->read_char()) != ' ' && let != '\n' && let != EOF)
    str += let;
  return str;
}
