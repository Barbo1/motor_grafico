#include "../../../../headers/concepts/gui.hpp"
    
TextBox::~TextBox () {
  delete [] this->text;
}
