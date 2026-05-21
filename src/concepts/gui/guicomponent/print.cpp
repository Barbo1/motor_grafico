#include "../../../../headers/concepts/gui.hpp"

void GuiComponent::print () {
  for (int32_t i = 0; i < this->many_elems; i++) {
    switch (this->elems[i].type) {
      case GUITypeButton: {
        Button* button = static_cast<Button*>(this->elems[i].ptr);
        switch (this->elems[i].state) {
          case GUIStateSelected: 
            button->selected_fn(this->position + button->position); 
            break;
          case GUIStateObserverd: 
            button->observed_fn(this->position + button->position); 
            break;
          case GUIStateQuiet: 
            button->quiet_fn(this->position + button->position); 
            break;
        }
      } 
      break;
      case GUITypeCheckBox: {
        CheckBox* check = static_cast<CheckBox*>(this->elems[i].ptr);
        if (check->active)
          check->active_fn(this->position + check->position); 
        else
          check->deactive_fn(this->position + check->position); 
      }
      break;
      case GUITypeSlider: {
        Slider* slider = static_cast<Slider*>(this->elems[i].ptr);
        Dir2 aux = this->position + slider->position;
        slider->base_fn(aux);

        Dir2 delta;
        switch (slider->direction) {
          case SliderDirectionX:
            delta = Dir2(slider->base_dims.x , 0.f);
            break;
          case SliderDirectionY:
            delta = Dir2(0.f, slider->base_dims.y);
            break;
          default:
            delta = Dir2(1.f, 0.f);
        }
        float distance = static_cast<float>(slider->curr_index) / slider->max_index;
        slider->sign_fn(delta.madd(distance - 0.5f, aux));
      }
      break;
      case GUITypeTextBox: {
        TextBox* textbox = static_cast<TextBox*>(this->elems[i].ptr);
        Dir2 aux = this->position + textbox->position;
        textbox->background(aux);

        Dir2 v = Dir2 (-textbox->dims.x, 0.f);
        Dir2 P = v.madd(0.5f, aux);
        std::string str = textbox->get_text();
        textbox->gs->print(
          str, 
          textbox->letter_size, 
          textbox->letter_color, 
          P
        );

        uint32_t total_length = textbox->gs->get_length(str, textbox->curr_pos, textbox->letter_size);
        textbox->cursor_image.draw(this->glb, P + Dir2(total_length + 2 * textbox->cursor_dev, 0.f));
      }
      break;
      default: break;
    }
  }
}
