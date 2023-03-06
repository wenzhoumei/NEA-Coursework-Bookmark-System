#pragma once
#include "component_controller.hpp"

class SelectedOptionPositionController: public ComponentController {
public:
    SelectedOptionPositionController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}
    void Down() {
	if (menu_data_.Option_List->GetSearched().size() == 0) { return; }
	if (menu_data_.SelectedOptionPosition == Max_()) { return; }
	if (menu_data_.SelectedOptionPosition > Max_()) {
	    menu_data_.SelectedOptionPosition = Max_();
	    return;
	}

	ChangePos(menu_data_.SelectedOptionPosition + 1);
    }

    void Up() {
	if (menu_data_.SelectedOptionPosition == 0) { return; }

	ChangePos(menu_data_.SelectedOptionPosition - 1);
    }

    void Reset() {
	ChangePos(0);
    }

private:
    size_t Max_() {
	if (menu_data_.Mode == MenuData::INSERT) {
	    return menu_data_.Option_List->GetSearched().size();
	} else {
	    return menu_data_.Option_List->GetSearched().size() - 1;
	}
    }

    void ChangePos(size_t new_position) {
	menu_data_.SelectedOptionPosition = new_position;

	if (menu_data_.Mode == MenuData::INSERT) {
	    menu_data_.Input = menu_data_.Option_List->At(menu_data_.SelectedOptionPosition);
	    menu_data_.Changed.Option_List = true;
	    menu_data_.Changed.Input = true;
	} else if (menu_data_.Mode == MenuData::EDIT) {
	    menu_data_.Input = menu_data_.Option_List->At(menu_data_.SelectedOptionPosition);
	    menu_data_.Changed.Input = true;
	}
    }
};
