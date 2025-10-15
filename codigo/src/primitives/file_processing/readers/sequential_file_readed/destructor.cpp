#include "../../../../../headers/primitives/file_processing.hpp"

SequentialFileReader::~SequentialFileReader () {
  this->fil.close();
}
