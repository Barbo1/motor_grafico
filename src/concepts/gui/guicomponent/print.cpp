#include "../../../../headers/concepts/gui.hpp"
#include <cstdint>

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
          default:
            std::unreachable();
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

        int xdevi = (textbox->config & TextBox::TextBoxConfig::TBCWinAgRight) && (textbox->xdev > 0.f) ? 
          static_cast<int>(textbox->xdev) + 2*textbox->cursor_dev: 
          0;
        Dir2 Q = textbox->dims.nmadd(0.5f, aux);
        SDL_Rect src = SDL_Rect {
          .x = xdevi,
          .y = 0, 
          .w = static_cast<int>(textbox->dims.x),
          .h = static_cast<int>(textbox->dims.y) 
        };
        SDL_Rect dst = SDL_Rect {
          .x = static_cast<int>(Q.x),
          .y = static_cast<int>(Q.y), 
          .w = static_cast<int>(textbox->dims.x),
          .h = static_cast<int>(textbox->dims.y) 
        };
        SDL_SetTextureBlendMode (textbox->text_area, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy (glb->get_render(), textbox->text_area, &src, &dst);
      }
      break;
      case GUITypeLabel: {
        Label* label = static_cast<Label*>(this->elems[i].ptr);
        Dir2 aux = this->position + label->position;
        label->background(aux);

        Dir2 Q = label->dims.nmadd(0.5f, aux);
        SDL_Rect dst = SDL_Rect {
          .x = static_cast<int>(Q.x),
          .y = static_cast<int>(Q.y), 
          .w = static_cast<int>(label->dims.x),
          .h = static_cast<int>(label->dims.y) 
        };
        SDL_SetTextureBlendMode (label->text_area, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy (glb->get_render(), label->text_area, nullptr, &dst);
      }
      break;
      default: std::unreachable();
    }
  }
}
