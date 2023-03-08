#include "menu_controller.hpp"
#include "parser.hpp"
#include "exit_code.hpp"

int MenuController::Enter() {
	if (menu_data_.Mode == MenuData::SEARCH) {
	    std::wstring chosen_input;
	    if (menu_data_.IsSearchListEmpty()) {
		chosen_input = menu_data_.Input;
	    } else {
		chosen_input = menu_data_.SelectedOptionString();
	    }

	    return Parser::Instance().Execute(menu_data_.Action, chosen_input);
	} else if (menu_data_.Mode == MenuData::EDIT) {
	    Option_List->Update();
	    SetMode(MenuData::SEARCH);
	} else if (menu_data_.Mode == MenuData::INSERT) {
	    Option_List->Insert();
	    SetMode(MenuData::SEARCH);
	}

	return ExitCode::DontExit;
}
