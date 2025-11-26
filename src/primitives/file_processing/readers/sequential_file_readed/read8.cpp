#include "../../../../../headers/primitives/file_processing.hpp"
#include <cstdint>

uint8_t SequentialFileReader::read8 () {
  if (this->finished()) 
    return 0;

  this->pos++;
  this->curr_file_pos++;
  if (this->pos == SQT_BUFF_LIMIT) {
    this->fil.read (this->buffer, SQT_BUFF_LIMIT);
    this->pos = 0;
    this->lim = this->fil.gcount ();
    this->is_lim = this->fil.eof ();
  }

  return this->buffer[this->pos];
}
