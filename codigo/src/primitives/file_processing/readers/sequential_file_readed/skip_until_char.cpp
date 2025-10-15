#include "../../../../../headers/primitives/file_processing.hpp"

void SequentialFileReader::skip_until (char stop) {
  while (!this->finished() && this->buffer[this->pos] != stop) {
    this->pos++;
    this->curr_file_pos++;
    if (this->pos >= SQT_BUFF_LIMIT) {
      this->fil.read(this->buffer, SQT_BUFF_LIMIT);
      this->pos = 0;
      this->lim = this->fil.gcount();
      this->is_lim = this->fil.eof();
    }
  }
} 
