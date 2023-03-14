#include "component_controller/selected_option_position_controller.hpp"

SelectedOptionPositionController::SelectedOptionPositionController(MenuData* menu_data): ComponentController(menu_data) {}

void SelectedOptionPositionController::Down() {
    if (Menu_Data_->Option_List->GetSearched().size() == 0) { return; }
    if (Menu_Data_->Selected_Option_Position == Max_()) { return; }
    if (Menu_Data_->Selected_Option_Position > Max_()) {
	Menu_Data_->Selected_Option_Position = Max_();
	return;
    }

    ChangePos(Menu_Data_->Selected_Option_Position + 1);
}

void SelectedOptionPositionController::Up() {
    if (Menu_Data_->Selected_Option_Position == 0) { return; }

    ChangePos(Menu_Data_->Selected_Option_Position - 1);
}

void SelectedOptionPositionController::Reset() {
    ChangePos(0);
}

size_t SelectedOptionPositionController::Max_() {
    if (Menu_Data_->Mode == MenuData::INSERT) {
	return Menu_Data_->Option_List->GetSearched().size();
    } else {
	return Menu_Data_->Option_List->GetSearched().size() - 1;
    }
}

void SelectedOptionPositionController::ChangePos(size_t new_position) {
    Menu_Data_->Selected_Option_Position = new_position;

    if (Menu_Data_->Mode == MenuData::INSERT) {
	Menu_Data_->Changed.Option_List = true;
	Menu_Data_->Changed.Input = true;
    } else if (Menu_Data_->Mode == MenuData::EDIT) {
	Menu_Data_->Input = Menu_Data_->SelectedName();
	Menu_Data_->Cursor_Position = Menu_Data_->Input.size();
	Menu_Data_->Changed.Input = true;
    } else if (Menu_Data_->SelectedMode == MenuData::DATA) {
	Menu_Data_->SelectedMode = MenuData::NAME;
    }
}
