#pragma once

#include "status_log_controller.hpp"
#include "title_controller.hpp"
#include "selected_option_position_controller.hpp"
#include "options_controller.hpp"
#include "cursor_position_controller.hpp"
#include "input_controller.hpp"

class MenuController {
public:
    MenuController(MenuData& menu_data) {
	StatusLog = new StatusLogController(menu_data);
	Title = new TitleController(menu_data, *StatusLog);

	SelectedOptionPosition = new SelectedOptionPositionController(menu_data, *StatusLog);
	Options = new OptionsController(menu_data, *StatusLog, *SelectedOptionPosition);

	CursorPosition = new CursorPositionController(menu_data, *StatusLog);
	Input = new InputController(menu_data, *StatusLog, *CursorPosition, *Options);
    }

    StatusLogController* StatusLog;
    TitleController* Title;
    CursorPositionController* CursorPosition;
    InputController* Input;
    SelectedOptionPositionController* SelectedOptionPosition;
    OptionsController* Options;
};
