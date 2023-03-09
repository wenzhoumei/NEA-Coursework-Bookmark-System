#include "menu_controller.hpp"
#include "parser.hpp"
#include "exit_code.hpp"

int MenuController::CtrlEnter() {
    return Parser::Instance().Execute(Menu_Data_.Option_List->GetAction(), Menu_Data_.Input);
}

int MenuController::Enter() {
	if (Menu_Data_.Mode == MenuData::SEARCH) {
	    std::wstring chosen_input;
	    if (Menu_Data_.IsSearchListEmpty()) {
		chosen_input = Menu_Data_.Input;
	    } else {
		chosen_input = Menu_Data_.SelectedData();
	    }

	    return Parser::Instance().Execute(Menu_Data_.Option_List->GetAction(), chosen_input);
	} else if (Menu_Data_.Mode == MenuData::EDIT) {
	    Option_List->Update();
	    SetMode(MenuData::SEARCH);
	} else if (Menu_Data_.Mode == MenuData::INSERT) {
	    Option_List->Insert();
	    SetMode(MenuData::SEARCH);
	}

	return ExitCode::DontExit;
}
