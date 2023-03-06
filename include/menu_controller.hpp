#pragma once

#include "status_log_controller.hpp"
#include "title_controller.hpp"
#include "selected_option_position_controller.hpp"
#include "option_list_controller.hpp"
#include "cursor_position_controller.hpp"
#include "input_controller.hpp"

class MenuController {
public:
    MenuController(MenuData& menu_data)
	: menu_data_(menu_data)
    {
	Status_Log = new StatusLogController(menu_data);
	Title = new TitleController(menu_data, *Status_Log);

	Selected_Option_Position = new SelectedOptionPositionController(menu_data, *Status_Log);
	Option_List = new OptionListController(menu_data, *Status_Log, *Selected_Option_Position);

	Cursor_Position = new CursorPositionController(menu_data, *Status_Log);
	Input = new InputController(menu_data, *Status_Log, *Cursor_Position, *Option_List);
    }

    void SetMode(enum MenuData::Mode m) {
	if (m == MenuData::EDIT) {
	    menu_data_.Mode = m;
	    Input->SetTextToSelected();
	} else if (m == MenuData::INSERT) {
	    menu_data_.Mode = m;
	    Input->Clear();
	    menu_data_.Changed.Option_List = true;
	} else if (m == MenuData::SEARCH) {
	    if (menu_data_.Mode != MenuData::SEARCH) {
		menu_data_.Mode = MenuData::SEARCH;
		Input->SetText(L"");
		Selected_Option_Position->Reset();
	    }
	}
    }

    StatusLogController* Status_Log;
    TitleController* Title;
    CursorPositionController* Cursor_Position;
    InputController* Input;
    SelectedOptionPositionController* Selected_Option_Position;
    OptionListController* Option_List;

private:
    MenuData& menu_data_;
};
