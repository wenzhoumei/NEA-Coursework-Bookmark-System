#pragma once
#include "component_controller.hpp"
#include "cursor_position_controller.hpp"
#include "option_list_controller.hpp"

class InputController: public ComponentController {
public:
    InputController(MenuData& menu_data, StatusLogController& status_log_controller, CursorPositionController& cursor_position_controller, OptionListController& options_controller): ComponentController(menu_data, status_log_controller), cursor_position_controller_(cursor_position_controller), options_controller_(options_controller) {}


    /**
     * @brief Adds character in cursor position
     * @param c is character to add
     */
    void AddChar(wchar_t c) {
	if (!iswprint(c)) return;

	menu_data_.Input.insert(menu_data_.Cursor_Position, 1, c);
	menu_data_.Changed.Input = true;

	cursor_position_controller_.Right();

	if (menu_data_.Mode == MenuData::SEARCH) {
	    options_controller_.Search();
	}
    }


    /**
     * @brief Removes last character from input box if it is not empty
     */
    void PopChar() {
	if (menu_data_.Input == L"") return;

	cursor_position_controller_.Left();
	menu_data_.Input.pop_back();
	menu_data_.Changed.Input = true;

	if (menu_data_.Mode == MenuData::SEARCH) {
	    options_controller_.Search();
	}
    }

    /**
     * @brief Sets text in input box
     * @param input_text is text to set it as
     */
    void SetText(std::wstring input_text) { 
	menu_data_.Input = input_text;
	
	cursor_position_controller_.MoveEnd();
	menu_data_.Changed.Input = true;

	if (menu_data_.Mode == MenuData::SEARCH) {
	    options_controller_.Search();
	}
    }

    /**
     * @brief Clears all text in input box
     */
    void Clear() { 
	SetText(L"");
    }

    void SetTextToSelected() {
	if (menu_data_.Option_List->GetSearched().size() == 0) { return; }

	SetText(menu_data_.SelectedName());
    }

    void SetTextToData() {
	if (menu_data_.Option_List->GetSearched().size() == 0) { return; }

	SetText(menu_data_.SelectedData());
    }
protected:
    CursorPositionController& cursor_position_controller_;
    OptionListController& options_controller_;
};
