#include "../../../../../headers/primitives/file_processing.hpp"
#include <iostream>

void SequentialFileReader::set_file_position (std::size_t new_size) {
  this->pos = 0;
  this->curr_file_pos = new_size - 1;
  this->fil.seekg (new_size - 1);
  this->fil.read (this->buffer, SQT_BUFF_LIMIT);
  this->fil.clear(this->fil.rdstate() & ~std::ios_base::failbit);
  this->lim = this->fil.gcount();
  this->is_lim = this->fil.eof();
} 
