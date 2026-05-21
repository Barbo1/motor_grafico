#include "../../../../headers/concepts/gui.hpp"

// methods for elements.
GuiElementType GuiComponent::get_type(uint32_t pos) const {
  return this->elems[pos].type;
}

GuiElementState GuiComponent::get_state(uint32_t pos) const {
  return this->elems[pos].state;
}

void GuiComponent::set_position(const Dir2& pos) {
  this->position = pos;
}

bool GuiComponent::any_selected () const {
  return this->id_selected > -1;
}

Dir2 GuiComponent::get_position() const {
  return this->position;
}
