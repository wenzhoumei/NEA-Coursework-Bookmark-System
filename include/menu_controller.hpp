#pragma once

#include <functional>
#include "menu_view.hpp"

#include "status_log_controller.hpp"
#include "title_controller.hpp"
#include "selected_option_position_controller.hpp"
#include "option_list_controller.hpp"
#include "cursor_position_controller.hpp"
#include "input_controller.hpp"

class KeyBinds;

class MenuController {
friend class KeyBinds;
public:
    MenuController(MenuData& menu_data, MenuView& menu_view)
	: Menu_Data(menu_data), Menu_View_(menu_view)
    {
	Status_Log = std::make_unique<StatusLogController>(StatusLogController(Menu_Data));
	Title = std::make_unique<TitleController>(TitleController(Menu_Data, *Status_Log));

	Selected_Option_Position = std::make_unique<SelectedOptionPositionController>(SelectedOptionPositionController(Menu_Data, *Status_Log));
	Option_List = std::make_unique<OptionListController>(OptionListController(Menu_Data, *Status_Log, *Selected_Option_Position));

	Cursor_Position = std::make_unique<CursorPositionController>(CursorPositionController(Menu_Data, *Status_Log));
	Input = std::make_unique<InputController>(InputController(Menu_Data, *Status_Log, *Cursor_Position, *Option_List));

	//Mode = std::make_unique<ModeController>(this);
    }



    void SetSearchMode() {
	Status_Log = std::make_unique<StatusLogController>(StatusLogController(Menu_Data));
	Title = std::make_unique<TitleController>(TitleController(Menu_Data, *Status_Log));

	Selected_Option_Position = std::make_unique<SelectedOptionPositionController>(SelectedOptionPositionController(Menu_Data, *Status_Log));
	Option_List = std::make_unique<OptionListController>(OptionListController(Menu_Data, *Status_Log, *Selected_Option_Position));

	Cursor_Position = std::make_unique<CursorPositionController>(CursorPositionController(Menu_Data, *Status_Log));
	Input = std::make_unique<InputController>(InputController(Menu_Data, *Status_Log, *Cursor_Position, *Option_List));

    }

    void SetEditMode() {
	if (Menu_Data.Option_List->Editable) {
	    Menu_Data.Mode = MenuData::EDIT;

	    Status_Log = std::make_unique<StatusLogController>(StatusLogController(Menu_Data));
	    Title = std::make_unique<TitleController>(TitleController(Menu_Data, *Status_Log));

	    Selected_Option_Position = std::make_unique<SelectedOptionPositionController>(SelectedOptionPositionController(Menu_Data, *Status_Log));
	    Option_List = std::make_unique<OptionListController>(OptionListController(Menu_Data, *Status_Log, *Selected_Option_Position));

	    Cursor_Position = std::make_unique<CursorPositionController>(CursorPositionController(Menu_Data, *Status_Log));
	    Input = std::make_unique<InputController>(InputController(Menu_Data, *Status_Log, *Cursor_Position, *Option_List));
	    if (Menu_Data.Editing == MenuData::NAME) {
		Input->SetTextToSelected();
	    } else {
		Input->SetTextToData();
	    }
	}
    }

    void SetInsertMode() {
	if (Menu_Data.Option_List->Editable) {
	    Menu_Data.Mode = MenuData::INSERT;

	    Status_Log = std::make_unique<StatusLogController>(StatusLogController(Menu_Data));
	    Title = std::make_unique<TitleController>(TitleController(Menu_Data, *Status_Log));

	    Selected_Option_Position = std::make_unique<SelectedOptionPositionController>(SelectedOptionPositionController(Menu_Data, *Status_Log));
	    Option_List = std::make_unique<OptionListController>(OptionListController(Menu_Data, *Status_Log, *Selected_Option_Position));

	    Cursor_Position = std::make_unique<CursorPositionController>(CursorPositionController(Menu_Data, *Status_Log));
	    Input = std::make_unique<InputController>(InputController(Menu_Data, *Status_Log, *Cursor_Position, *Option_List));
	    Input->Clear();
	    Menu_Data.Changed.Option_List = true;
	}
    }

    void SetMode(enum MenuData::Mode m) {
	if (m == MenuData::EDIT) {
	    Menu_Data.Mode = m;
	    if (Menu_Data.Editing == MenuData::NAME) {
		Input->SetTextToSelected();
	    } else {
		Input->SetTextToData();
	    }
	} else if (m == MenuData::INSERT) {
	    Menu_Data.Mode = m;
	    Input->Clear();
	    Menu_Data.Changed.Option_List = true;
	} else if (m == MenuData::SEARCH) {
	    if (Menu_Data.Mode != MenuData::SEARCH) {
		Selected_Option_Position->Reset();
		Menu_Data.Mode = MenuData::SEARCH;
		Input->SetText(L"");
	    }
	}
    }

    void ToggleData() const {
    }

    int CtrlEnter();
    int Enter();

    std::unique_ptr<StatusLogController> Status_Log;
    std::unique_ptr<TitleController> Title;
    std::unique_ptr<CursorPositionController> Cursor_Position;
    std::unique_ptr<InputController> Input;
    std::unique_ptr<SelectedOptionPositionController> Selected_Option_Position;
    std::unique_ptr<OptionListController> Option_List;

    MenuData& Menu_Data;
    MenuView& Menu_View;
};
