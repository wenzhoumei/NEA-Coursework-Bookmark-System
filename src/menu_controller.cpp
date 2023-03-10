#include "menu_controller.hpp"
#include "exit_code.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
#include <ncurses.h>

MenuController::MenuController(MenuData& menu_data)
    : Menu_Data_(menu_data) {}

int MenuController::ProcessChar(const wchar_t& c) {
    PossibleExit p_e = ProcessPossibleExit_(c);
    if (p_e.Matched) { return p_e.ReturnCode; }
    if (ProcessSpecialChars_(c).Matched) { return ExitCode::DontExit; }

    ProcessDefaultChar_(c);
    return ExitCode::DontExit;
}

void MenuController::SetTitle() {
    Menu_Data_.Title = Menu_Data_.Option_List->GetAction() + Menu_Data_.Option_List->GetAction();
}

/*
MenuController::MenuController(MenuData& menu_data)
	: Menu_Data_(menu_data)
    {
	Status_Log = new StatusLogController(menu_data);
	Title = new TitleController(menu_data, *Status_Log);

	Selected_Option_Position = new SelectedOptionPositionController(menu_data, *Status_Log);
	Option_List = new OptionListController(menu_data, *Status_Log, *Selected_Option_Position);

	Cursor_Position = new CursorPositionController(menu_data, *Status_Log);
	Input = new InputController(menu_data, *Status_Log, *Cursor_Position, *Option_List);
    }


    int MenuController::ProcessChar(const wchar_t& choice) {
	switch (choice) {
	    case KEY_DOWN:
		Selected_Option_Position->Down();
		break;
	    case KEY_UP:
		Selected_Option_Position->Up();
		break;
	    case KEY_LEFT:
		Cursor_Position->Left();
		break;
	    case KEY_RIGHT:
		Cursor_Position->Right();
		break;
	    case CTRL_MASK('a'):
		Option_List->Add();
		break;
	    case CTRL_MASK('r'):
		Option_List->Remove();
		break;
	    case CTRL_MASK('k'):
		SetMode(MenuData::INSERT);
		break;
	    case CTRL_MASK('e'):
		SetMode(MenuData::EDIT);
		break;
	    case CTRL_MASK('d'):
		ToggleData();
		break;
	    case KEY_BACKSPACE:
	    case 127:  // Some systems use 127 instead of KEY_BACKSPACE
		Input->PopChar();
		break;
	    case KEY_ESCAPE:
		if (Menu_Data_.Mode != MenuData::SEARCH) {
		    SetMode(MenuData::SEARCH);
		} else {
		    return 0;
		}
		break;
	    case CTRL_MASK(KEY_ENTER):
		return CtrlEnter();
		break;
	    case KEY_ENTER:
	    case '\n':
		return Enter();
		break;
	    case '\t':
		Input->SetTextToSelected();
		break;
	    default:
		Input->AddChar(choice);
		break;
	}

	return ExitCode::DontExit;
    }

    void MenuController::SetMode(enum MenuData::Mode m) {
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

    void MenuController::ToggleData() {
	if (Menu_Data_.Editing == MenuData::NAME) { Menu_Data_.Editing = MenuData::DATA; }
	else if (Menu_Data_.Editing == MenuData::DATA) { Menu_Data_.Editing = MenuData::NAME; }
    }
*/
