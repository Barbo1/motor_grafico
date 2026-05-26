#include "../../../../headers/concepts/gui.hpp"
    
Label::~Label () {
  delete [] this->text;
}
