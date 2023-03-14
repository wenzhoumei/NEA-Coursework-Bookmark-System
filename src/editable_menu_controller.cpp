#include "menu_controller/editable_menu_controller.hpp"
#include "exit_code.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
#include <ncurses.h>

MenuController::PossibleExit EditableMenuController::ProcessPossibleExit_(const wchar_t& c) {
    bool matched = true;
    int ret;

    switch (c) {
	case KEY_ESCAPE:
	    if (Menu_Data_.SelectedMode == MenuData::DATA) {
		ReadOnlyMenuController::ToggleData_();
		SetMode_(MenuData::SEARCH);
		Input_.SetTextToSelected();
		ret = ExitCode::DontExit;
	    } else if (Menu_Data_.Mode == MenuData::SEARCH) {
		return ReadOnlyMenuController::ProcessPossibleExit_(c);
		break;
	    } else if (Menu_Data_.Mode == MenuData::EDIT) {
		SetMode_(MenuData::SEARCH);
		ret = ExitCode::DontExit;
	    } else if (Menu_Data_.Mode == MenuData::INSERT) {
		SetMode_(MenuData::SEARCH);
		ret = ExitCode::DontExit;
	    }
	    break;
	case KEY_ENTER:
	case '\n':
	{
	    if (Menu_Data_.Mode == MenuData::SEARCH) {
		return ReadOnlyMenuController::ProcessPossibleExit_(c);
		break;
	    } else if (Menu_Data_.Mode == MenuData::EDIT) {
		Option_List_.Update();
		SetMode_(MenuData::SEARCH);
	    } else if (Menu_Data_.Mode == MenuData::INSERT) {
		Option_List_.Insert();
		SetMode_(MenuData::SEARCH);
	    }

	    ret = ExitCode::DontExit;
	    break;
	}

	default:
	    matched = false;
    }

    if (!matched) { return ReadOnlyMenuController::ProcessPossibleExit_(c); }
    else return { matched, ret };
}

MenuController::SpecialChar EditableMenuController::ProcessSpecialChars_(const wchar_t& c) {
    bool matched = true;

    switch (c) {
	case CTRL_MASK('a'):
	    Option_List_.Add();
	    break;
	case CTRL_MASK('r'):
	    Option_List_.Remove();
	    break;
	case CTRL_MASK('k'):
	    SetMode_(MenuData::INSERT);
	    break;
	case CTRL_MASK('e'):
	    SetMode_(MenuData::EDIT);
	    break;
	default:
	    matched = false;
    }

    if (!matched) { return ReadOnlyMenuController::ProcessSpecialChars_(c); }
    return { matched };
}

void EditableMenuController::SetMode_(enum MenuData::Mode m) {
    if (m == MenuData::EDIT) {
	Menu_Data_.Mode = m;
	if (Menu_Data_.SelectedMode == MenuData::NAME) {
	    Input_.SetTextToSelected();
	} else {
	    Input_.SetTextToData();
	}
    } else if (m == MenuData::INSERT) {
	Menu_Data_.Mode = m;
	Input_.Clear();
	Menu_Data_.Changed.Option_List = true;
    } else if (m == MenuData::SEARCH) {
	if (Menu_Data_.Mode != MenuData::SEARCH) {
	    Selected_Option_Position_.Reset();
	    Menu_Data_.Mode = MenuData::SEARCH;
	    Input_.SetText(L"");
	}
    }

    SetTitle();
}

void EditableMenuController::SetTitle() {
    if (Menu_Data_.Mode == MenuData::SEARCH) {
	Title_.SetText(L"[S] " + Menu_Data_.Option_List->GetLocation() + Menu_Data_.Option_List->GetActionToHere());
    } else if (Menu_Data_.Mode == MenuData::EDIT) {
	Title_.SetText(L"[E] " + Menu_Data_.Option_List->GetLocation() + Menu_Data_.Option_List->GetActionToHere());
    } else if (Menu_Data_.Mode == MenuData::INSERT) {
	Title_.SetText(L"[I] " + Menu_Data_.Option_List->GetLocation() + Menu_Data_.Option_List->GetActionToHere());
    }
}
