#include "../../../../headers/concepts/gui.hpp"

void GuiComponent::add (Button* button) {
  this->elems[this->many_elems] = GuiElement {
    .ptr = button, 
    .type = GUITypeButton, 
    .state = GUIStateQuiet
  };
  this->many_elems++;
}

void GuiComponent::add (CheckBox* checkbox) {
  this->elems[this->many_elems] = GuiElement {
    .ptr = checkbox, 
    .type = GUITypeCheckBox, 
    .state = GUIStateQuiet
  };
  this->many_elems++;
}

void GuiComponent::add (Slider* slider) {
  this->elems[this->many_elems] = GuiElement {
    .ptr = slider, 
    .type = GUITypeSlider, 
    .state = GUIStateQuiet
  };
  this->many_elems++;
}

void GuiComponent::add (TextBox* txtb) {
  this->elems[this->many_elems] = GuiElement {
    .ptr = txtb, 
    .type = GUITypeTextBox, 
    .state = GUIStateQuiet
  };
  this->many_elems++;
}
