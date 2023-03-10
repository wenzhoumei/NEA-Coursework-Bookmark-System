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
	: Menu_Data_(menu_data)
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
	    Menu_Data_.Mode = m;
	    if (Menu_Data_.Editing == MenuData::NAME) {
		Input->SetTextToSelected();
	    } else {
		Input->SetTextToData();
	    }
	} else if (m == MenuData::INSERT) {
	    Menu_Data_.Mode = m;
	    Input->Clear();
	    Menu_Data_.Changed.Option_List = true;
	} else if (m == MenuData::SEARCH) {
	    if (Menu_Data_.Mode != MenuData::SEARCH) {
		Selected_Option_Position->Reset();
		Menu_Data_.Mode = MenuData::SEARCH;
		Input->SetText(L"");
	    }
	}
    }

    void ToggleData() {
	if (Menu_Data_.Editing == MenuData::NAME) { Menu_Data_.Editing = MenuData::DATA; }
	else if (Menu_Data_.Editing == MenuData::DATA) { Menu_Data_.Editing = MenuData::NAME; }
    }

    int Enter();
    int CtrlEnter();

    StatusLogController* Status_Log;
    TitleController* Title;
    CursorPositionController* Cursor_Position;
    InputController* Input;
    SelectedOptionPositionController* Selected_Option_Position;
    OptionListController* Option_List;

private:
    MenuData& Menu_Data_;
};
