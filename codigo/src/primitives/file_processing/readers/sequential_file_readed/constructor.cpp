#include "../../../../../headers/primitives/file_processing.hpp"

SequentialFileReader::SequentialFileReader (std::string path, int * error) {
  this->path = path;
  this->fil = std::ifstream (path, std::ios::in | std::ios::binary);

  if (!fil.is_open()) {
    *error = -1;
    this->curr_file_pos = this->lim_file_pos = this->lim = this->pos = 0;
    this->is_lim = true;
  } else {
    *error = 0;
    this->pos = SQT_BUFF_LIMIT - 1;
    this->curr_file_pos = 0;
    this->is_lim = false;
    this->fil.seekg(std::ios_base::end);
    this->lim_file_pos = this->fil.tellg();
    this->fil.seekg(std::ios_base::beg);
  }
}
