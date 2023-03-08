#include "menu_tui.hpp"
#include "exit_code.hpp"

MenuTUI::MenuTUI(std::unique_ptr<OptionList> option_list, const std::wstring& action, const std::wstring& data)
{
    menu_data_ = std::make_unique<MenuData>(MenuData(data + action, action, std::move(option_list)));
}

int MenuTUI::Open() {
    menu_view_ = std::make_unique<MenuView>(MenuView(menu_data_.get()));
    if (menu_data_->Option_List->SuccessfullyLoaded()) {
	menu_controller_ = std::make_unique<MenuController>(MenuController(*menu_data_));
    } else {
	//TODO
	menu_controller_ = std::make_unique<MenuController>(MenuController(*menu_data_));
	//menu_controller_ = std::make_unique<MenuController>(MenuController(*menu_data_));
	//menu_controller_ = std::make_unique<FailedMenuController>(FailedMenuController(*menu_data_));
    }

    set_escdelay(0);

    menu_view_->Start();
    menu_controller_->Option_List->Search();
    menu_view_->Display();

    int exit_code;
    while ((exit_code = GetChar_()) == ExitCode::DontExit) {
	menu_view_->Display();
    }

    return exit_code;
}

MenuController* MenuTUI::GetController() {
    return menu_controller_.get();
}

int MenuTUI::GetChar_() {
    wint_t choice;
    get_wch(&choice);

    switch (choice) {
	case KEY_DOWN:
	    menu_controller_->Selected_Option_Position->Down();
	    break;
	case KEY_UP:
	    menu_controller_->Selected_Option_Position->Up();
	    break;
	case KEY_LEFT:
	    menu_controller_->Cursor_Position->Left();
	    break;
	case KEY_RIGHT:
	    menu_controller_->Cursor_Position->Right();
	    break;
	case CTRL_MASK('a'):
	    menu_controller_->Option_List->Add();
	    break;
	case CTRL_MASK('r'):
	    menu_controller_->Option_List->Remove();
	    break;
	case CTRL_MASK('k'):
	    menu_controller_->SetMode(MenuData::INSERT);
	    break;
	case CTRL_MASK('e'):
	    menu_controller_->SetMode(MenuData::EDIT);
	    break;
	case CTRL_MASK('d'):
	    menu_controller_->ToggleData();
	    break;
	case KEY_BACKSPACE:
	case 127:  // Some systems use 127 instead of KEY_BACKSPACE
	    menu_controller_->Input->PopChar();
	    break;
	case KEY_ESCAPE:
	    if (menu_data_->Mode != MenuData::SEARCH) {
		menu_controller_->SetMode(MenuData::SEARCH);
	    } else {
		return 0;
	    }
	    break;
	case KEY_ENTER:
	case '\n':
	    return menu_controller_->Enter();
	    break;
	case '\t':
	    menu_controller_->Input->SetTextToSelected();
	    break;
	default:
	    menu_controller_->Input->AddChar(choice);
	    break;
    }

    return ExitCode::DontExit;
}
