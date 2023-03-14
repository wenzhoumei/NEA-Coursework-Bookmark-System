#include "menu_controller/read_only_menu_controller.hpp"
#include "exit_code.hpp"
#include "parser.hpp"
#include "menu_data.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
#include <ncurses.h>
#include "log.hpp"

ReadOnlyMenuController::ReadOnlyMenuController(MenuData& menu_data)
	: MenuController(menu_data),
	  Title_(&menu_data),
	  Cursor_Position_(&menu_data),
	  Selected_Option_Position_(&menu_data),
	  Option_List_(&menu_data, Selected_Option_Position_),
	  Input_(&menu_data, Cursor_Position_, Option_List_)
    {
    }

MenuController::PossibleExit ReadOnlyMenuController::ProcessPossibleExit_(const wchar_t& c) {
    MenuController::PossibleExit p_e { true, ExitCode::LogicError };

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
	case CTRL_MASK('b'): ///< Go back in history
	    if (MenuData::History.size() == 1) {
		p_e.ReturnCode = ExitCode::DontExit;
		my::log.Info() << "Bottom of history stack" << std::endl;
	    } else {
		MenuData::History.pop();
		std::pair<std::wstring, std::wstring> last_menu = MenuData::History.top();
		MenuData::History.pop();

		p_e.ReturnCode = Parser::Instance().Execute(last_menu.first, last_menu.second);
	    }
	    break;
	default:
	    p_e.Matched = false;
    }

    return p_e;
}

MenuController::SpecialChar ReadOnlyMenuController::ProcessSpecialChars_(const wchar_t& c) {
    bool matched = true;

    switch (c) {
	case CTRL_MASK('a'):
	    my::log.Info() << "Can't add, read only" << std::endl;
	    break;
	case CTRL_MASK('r'):
	    my::log.Info() << "Can't remove, read only" << std::endl;
	    break;
	case CTRL_MASK('k'):
	    my::log.Info() << "Can't enter insert mode, read only" << std::endl;
	    break;
	case CTRL_MASK('e'):
	    my::log.Info() << "Can't enter edit mode, read only" << std::endl;
	    break;
	case CTRL_MASK('v'):
	    Input_.Paste();
	    break;
	case CTRL_MASK('c'):
	    Input_.Copy();
	    break;
	case CTRL_MASK('q'):
	    Input_.Clear();
	    break;
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
	    Input_.Backspace();
	    break;
	case '\t':
	    if (Menu_Data_.Mode == MenuData::SEARCH) {
		Input_.SetTextToSelected();
	    } else {
		Input_.Autofill();
	    }
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
    switch (c) {
	case KEY_RESIZE:
	    break;
	default:
	    if (iswprint(c)) {
		Input_.AddChar(c);
	    }
    }
}

void ReadOnlyMenuController::ToggleData_() {
    if (Menu_Data_.Option_List->IsBookmarkList()) {
	if (Menu_Data_.Editing == MenuData::DATA) { Menu_Data_.Editing = MenuData::NAME; }
	else { Menu_Data_.Editing = MenuData::DATA; }
    } else {
	my::log.Info() << "Not a bookmark list" << std::endl;
    }
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
	    Was_Input_ = true;
	    return Parser::Instance().Execute(Menu_Data_.Option_List->GetActionOutOfHere(), chosen_input);
	} else {
	    Was_Input_ = false;
	    chosen_input = Menu_Data_.SelectedOptionString();
	}

	return Parser::Instance().Execute(Menu_Data_.Option_List->GetActionOutOfHere(), chosen_input);
}

void ReadOnlyMenuController::SetTitle() {
    Title_.SetText(L"(S) " + Menu_Data_.Option_List->GetLocation() + Menu_Data_.Option_List->GetActionToHere());
}
