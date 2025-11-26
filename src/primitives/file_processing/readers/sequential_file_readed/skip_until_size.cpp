#include "../../../../../headers/primitives/file_processing.hpp"

void SequentialFileReader::skip_until_n (std::size_t n) {
  this->pos += n;
  this->curr_file_pos += n;
  if (this->pos >= SQT_BUFF_LIMIT && !this->finished()) {
    this->fil.seekg(this->pos - SQT_BUFF_LIMIT, std::ios_base::cur);
    this->fil.read(this->buffer, SQT_BUFF_LIMIT);
    this->pos = 0;
    this->lim = this->fil.gcount();
    this->is_lim = this->fil.eof();
  }
} 
