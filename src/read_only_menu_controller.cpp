#include "menu_controller/read_only_menu_controller.hpp"
#include "exit_code.hpp"
#include "parser.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
#include <ncurses.h>

MenuController::PossibleExit ReadOnlyMenuController::ProcessPossibleExit_(const wchar_t& c) {
    MenuController::PossibleExit p_e { true, 0 };

    switch (c) {
	case CTRL_MASK(KEY_ENTER):
	    p_e.ReturnCode = ExecuteInput_();
	    break;
	case KEY_ENTER:
	case '\n':
	    p_e.ReturnCode = ExecuteSelected_();
	    break;
	case KEY_ESCAPE:
	    p_e.ReturnCode = ExitCode::Success;
	    break;
	default:
	    p_e.Matched = false;
    }

    return p_e;
}

MenuController::SpecialChar ReadOnlyMenuController::ProcessSpecialChars_(const wchar_t& c) {
    bool matched = true;

    switch (c) {
	case KEY_DOWN:
	    Selected_Option_Position_.Down();
	    break;
	case KEY_UP:
	    Selected_Option_Position_.Up();
	    break;
	case KEY_LEFT:
	    Cursor_Position_.Left();
	    break;
	case KEY_RIGHT:
	    Cursor_Position_.Right();
	    break;
	case KEY_BACKSPACE:
	case 127:  // Some systems use 127 instead of KEY_BACKSPACE
	    Input_.PopChar();
	    break;
	case '\t':
	    Input_.SetTextToSelected();
	    break;
	case CTRL_MASK('d'):
	    ToggleData_();
	    break;
	default:
	    matched = false;
    }

    return { matched };
}

void ReadOnlyMenuController::ProcessDefaultChar_(const wchar_t &c) {
    Input_.AddChar(c);
}

void ReadOnlyMenuController::ToggleData_() {
    if (Menu_Data_.Editing == MenuData::DATA) { Menu_Data_.Editing = MenuData::NAME; }
    else { Menu_Data_.Editing = MenuData::DATA; }
}

int ReadOnlyMenuController::ExecuteInput_() {
    if (Menu_Data_.Input == L"") { return ExitCode::DontExit; }
    return Parser::Instance().Execute(Menu_Data_.Option_List->GetActionOutOfHere(), Menu_Data_.Input);
}

int ReadOnlyMenuController::ExecuteSelected_() {
	std::wstring chosen_input;
	if (Menu_Data_.IsSearchListEmpty()) {
	    chosen_input = Menu_Data_.Input;
	    if (chosen_input == L"") { return ExitCode::DontExit; }
	} else {
	    chosen_input = Menu_Data_.SelectedName();
	}

	return Parser::Instance().Execute(Menu_Data_.Option_List->GetActionOutOfHere(), chosen_input);
}

void ReadOnlyMenuController::SetTitle() {
    Title_.SetText(L"(S) " + Menu_Data_.Option_List->GetLocation() + Menu_Data_.Option_List->GetActionToHere());
}
