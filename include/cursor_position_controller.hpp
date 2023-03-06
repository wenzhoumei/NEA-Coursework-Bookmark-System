#pragma once
#include "component_controller.hpp"

class CursorPositionController: public ComponentController {
public:
    CursorPositionController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}

    /**
     * Moves cursor to the right if possible
     */
    void Right() {
	if (menu_data_.Cursor_Position == menu_data_.Input.size()) { return; }
	if (menu_data_.Cursor_Position > menu_data_.Input.size()) {
	    menu_data_.Input = menu_data_.Input.size();
	    return;
	}

	menu_data_.Cursor_Position++;
    }

    /**
     * Moves cursor to the left if possible
     */
    void Left() {
	if (menu_data_.Cursor_Position <= 0) { return; }

	menu_data_.Cursor_Position--;
    }

    /**
     * Sets cursor position to end of input text
     */
    void MoveEnd() {
	menu_data_.Cursor_Position = menu_data_.Input.size();
    }
};
